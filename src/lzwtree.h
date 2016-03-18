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

#ifndef LZWTREE_H
#define LZWTREE_H

#include <memory>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

class lzwTree
{
    using Asd = std::vector<unsigned int>;

public:
    lzwTree();
    
    lzwTree& insert(std::string str);
    
    void insert(char ch);

    void calc();
    
    void print();
    
    //~lzwTree();

private:
    class Node
    {
    public:
        Node(char val) : m_val{ val }, m_left{ nullptr }, m_right{ nullptr }
        { 
        }
        
        char val() const { return m_val; }
        
        std::shared_ptr<Node> left() const { return m_left; }
        std::shared_ptr<Node> right() const { return m_right; }
        
        void setLeft(std::shared_ptr<Node> left) { m_left = left; };
        void setRight(std::shared_ptr<Node> right) { m_right = right;};
    private:
        char m_val;
        std::shared_ptr<Node> m_left;
        std::shared_ptr<Node> m_right;
    };
    
    std::shared_ptr<Node> m_root;
    std::shared_ptr<Node> m_inserter;
    
    unsigned int m_depth;
    double m_mean;
    double m_spread;
    unsigned int m_numOfLeaves;

    void calc(std::shared_ptr<Node> currentNode, Asd& asd);
    void print(std::shared_ptr<Node> currentNode, size_t& depth);
};

#endif // LZWTREE_H