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
            m_depth++;
        }
        else
        {
            std::shared_ptr<Node> tmp = std::make_shared<Node>('1');
            m_inserter->setRight(tmp);

            auto it = m_leaves.find(m_inserter);
            if (it != m_leaves.end())
            {
                m_leaves.erase(it);
            }

            m_leaves[m_inserter->right()] = m_depth + 1;

            m_inserter = m_root;
            m_depth = 0;
        }
    }
    else 
    {
        if (m_inserter->left())
        {
            m_inserter = m_inserter->left();
            m_depth++;
        }
        else
        {
            std::shared_ptr<Node> tmp = std::make_shared<Node>('0');
            m_inserter->setLeft(tmp);

            auto it = m_leaves.find(m_inserter);
            if (it != m_leaves.end())
            {
                m_leaves.erase(it);
            }

            m_leaves[m_inserter->left()] = m_depth + 1;

            m_inserter = m_root;
            m_depth = 0;
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
    std::for_each(m_leaves.begin(), m_leaves.end(), [&](const leafPair& l){ m_mean += l.second; });
    
    m_mean /= m_leaves.size();

    for (auto leaf : m_leaves)
    {
        m_spread += (leaf.second - m_mean) * (leaf.second - m_mean);
    }

    m_spread *= (1.0 / (m_leaves.size() - 1));
    m_spread = std::sqrt(m_spread);
    
    m_depth = std::max_element(m_leaves.begin(), m_leaves.end(), [](const leafPair& lhs, const leafPair& rhs){
                    return lhs.second < rhs.second;
                })->second;

    std::cout << "mean = " << m_mean << "\tspread = " << m_spread 
              << "\tdepth = " << m_depth << std::endl;
}
