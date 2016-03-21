/*
 * lzwTree
 * Copyright (C) 2016  Viktor Szilárd Simkó aqviktor@gmail.com
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

lzwTree::lzwTree() : m_depth{}, m_mean{}, m_spread{}
{
    m_root = std::make_shared<Node>('/');
    m_inserter = m_root;
}

lzwTree::lzwTree(lzwTree&& rhs)
{
    m_root = rhs.m_root;
    rhs.m_root = nullptr;
}

lzwTree& lzwTree::operator=(lzwTree&& rhs)
{
    m_root = rhs.m_root;
    rhs.m_root = nullptr;
    return *this;
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
    leafVec leaves;
    calc(m_root, leaves);
    
    std::for_each(leaves.begin(), leaves.end(), [&](unsigned int depth){ m_mean += depth; });
    
    m_mean /= leaves.size();

    for (auto leaf : leaves)
    {
        m_spread += (leaf - m_mean) * (leaf - m_mean);
    }

    m_spread *= (1.0 / (leaves.size() - 1));
    m_spread = std::sqrt(m_spread);
    
    m_depth = *(std::max_element(leaves.begin(), leaves.end()));

    std::cout << "mean = " << m_mean << "\tspread = " << m_spread 
              << "\tdepth = " << m_depth << std::endl;
}

void lzwTree::calc(std::shared_ptr<Node> currentNode, leafVec& leaves)
{
    if (currentNode == nullptr) return;

    m_depth++;

    if (!currentNode->left() && !currentNode->right())
    {
        //m_numOfLeaves++;
        //m_mean += --m_depth;
        leaves.push_back(--m_depth);

        return;
    }

    calc(currentNode->right(), leaves);

    calc(currentNode->left(), leaves);

    m_depth--;
}

/*
lzwTree::~lzwTree()
{

}
*/




