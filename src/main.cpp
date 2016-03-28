#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <boost/program_options.hpp>
#include "lzwtree.h"

/* 
 * This functor:
 * -- splits the bytes from the input into bits
 * -- inserts the bits into the tree as chars
 * -- informs about the progress of building the tree
 */
class reader
{
public:
    reader(lzwTree& tree, unsigned long size) : tree{ tree }, comment{ false }, counter{ }, checkpoint{ }, size{ size } 
    { 

    }

    void operator() (char ch)
    {

        if (counter++ == checkpoint)
        {
            // inform about the progress
            std::cerr << (checkpoint / (size / 10)) * 10 << "\% built." << std::endl;
            checkpoint += size / 10;
        }

        // comments begin with '>'
        if (ch == '>')
        {
            comment = true;
            return;
        }
        if (ch == '\n')
        {
            comment = false;
            return;
        }
        if (comment)
        {
            return;
        }
        if (ch == 'N') 
        {
            return;
        }

        // split the bytes into bits
        for (auto i = 0; i < 8; ++i)
        {
            if ((ch << i) & 0x80) // mask with 1000_0000 so we get the MSB
            {
                tree.insert('1');
            }
            else 
            {
                tree.insert('0');
            }
        }
    }
    
private:
    lzwTree& tree; // the tree into which we need to insert
    bool comment; // true, if we are in a comment
    unsigned long counter; // the number of bytes that are already processed
    unsigned long checkpoint; // every tenth of the the size of the input is a checkpoint
    unsigned long size; // the size of the input
};

int main(int argc, const char **argv)
{
    
    boost::program_options::options_description desc("Available options");
    
    desc.add_options()
        ("help", "help message")
        ("in", boost::program_options::value<std::string>(), "name of the input file")
    ;
    
    boost::program_options::variables_map vm;
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);
    
    boost::program_options::notify(vm);
    
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }
    
    std::string inFile;
    
    if (vm.count("in"))
    {
        inFile = vm["in"].as<std::string>();
        std::cerr << "Input file is: " << inFile << std::endl;
    }
    else
    {
        std::cerr << "Input file was not found." << std::endl;
        return 0;
    }   
    
    std::ifstream inputStream(inFile, std::ios::binary);

    if (inputStream.is_open()) std::cerr << "Input stream is open." << std::endl;
    
    std::vector<char> buffer(std::istreambuf_iterator<char>{inputStream},
                             std::istreambuf_iterator<char>{});

    std::cerr << buffer.size() << " bytes read." << std::endl;
    
    lzwTree tree;
    
    std::for_each(buffer.begin(), buffer.end(), reader{tree, buffer.size()});
    
    //tree.insert("01111001001001000111");
    tree.printInfo();
    tree.print();
    
    return 0;
}
