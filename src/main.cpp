#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <boost/program_options.hpp>
#include "lzwtree.h"

class reader
{
public:
    reader(lzwTree& tree) : tree{ tree }, comment{ false } { }

    void operator() (char ch)
    {
        //std::cerr << "wtf meghívva" << comment << std::endl;
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

        for (auto i = 0; i < 8; ++i)
        {
            if ((ch << i) & 0x80)
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
    lzwTree& tree;
    bool comment;
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
        std::cout << "Input file is: " << inFile << std::endl;
    }
    else
    {
        std::cout << "Input file was not found." << std::endl;
        return 0;
    }   
    
    std::ifstream inputStream(inFile, std::ios::binary);

    if (inputStream.is_open()) std::cerr << "stream is open" << std::endl;
    
    std::vector<char> buffer(std::istreambuf_iterator<char>{inputStream},
                             std::istreambuf_iterator<char>{});

    std::cerr << "beolvasva" << buffer.size() << "byte" << std::endl;
    
    lzwTree tree;
    
    std::for_each(buffer.begin(), buffer.end(), reader{tree});

    
    //tree.insert("01111001001001000111");
    tree.calc();
    //tree.print();
    
    return 0;
}
