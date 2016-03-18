/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "lzwtree.h"

lzwTree::lzwTree() : m_depth{}, m_mean{}, m_numOfLeaves{0}, m_spread{0}
{
    m_root = std::make_shared<Node>('/');
    m_inserter = m_root;
}

lzwTree& lzwTree::insert(std::string str)
{
    for (auto c : str) 
    {
        insert(c);
    }
    return *this;
}

void lzwTree::insert(char ch)
{
    if (ch == '1') 
    {
        if (m_inserter->right())
        {
            m_inserter = m_inserter->right();
        }
        else
        {
            std::shared_ptr<Node> tmp = std::make_shared<Node>('1');
            m_inserter->setRight(tmp);
            m_inserter = m_root;
        }
    }
    else 
    {
        if (m_inserter->left())
        {
            m_inserter = m_inserter->left();
        }
        else
        {
            std::shared_ptr<Node> tmp = std::make_shared<Node>('0');
            m_inserter->setLeft(tmp);
            m_inserter = m_root;
        }
    }
}

void lzwTree::print()
{
    size_t depth{};
    print(m_root, depth);
}

void lzwTree::print(std::shared_ptr<Node> currentNode, size_t& depth)
{
    if (!currentNode)
    {
        std::cout << std::endl;
        return;
    }
    
    print(currentNode->right(), ++depth);
    
    for (auto i = 0u; i < depth; ++i)
    {
        std::cout << "--";
    }
    
    std::cout << currentNode->val() << '(' << depth - 1 << ')' << std::endl;
    
    print(currentNode->left(), depth);
    
    depth--;
}

void lzwTree::calc()
{
    Asd asd;
    calc(m_root, asd);

    m_mean /= m_numOfLeaves;

    for (auto tmpAsd : asd)
    {
        m_spread += (tmpAsd - m_mean) * (tmpAsd - m_mean);
    }

    m_spread *= (1.0 / (m_numOfLeaves - 1));
    m_spread = std::sqrt(m_spread);

    std::cout << "mean = " << m_mean << "\tspread = " << m_spread 
              << "\tdepth = " << m_depth << std::endl;
}

void lzwTree::calc(std::shared_ptr<Node> currentNode, Asd& asd)
{
    if (currentNode == nullptr) return;

    m_depth++;

    if (!currentNode->left() && !currentNode->right())
    {
        m_numOfLeaves++;
        m_mean += --m_depth;
        asd.push_back(m_depth);

        return;
    }

    calc(currentNode->right(), asd);

    calc(currentNode->left(), asd);

    m_depth--;
}

/*
lzwTree::~lzwTree()
{

}
*/




