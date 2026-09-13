/*
 * Copyright 2026 Pie Laboratories
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GENERALTREENODE_DOT_HPP
#define GENERALTREENODE_DOT_HPP

#include <iterator>
#include <vector>
#include <memory>
#include <functional>
#ifndef BINARYLIST_DOT_HPP
    #include "data-structures/binary/BinaryList.hpp"
#endif

namespace DataStructures::Tree
{
    template<typename T>
    class GeneralTreeNode
    {
    public:
        typedef std::function<bool(const T&, const T&)> GeneralTreeNodeComparator;

        const T &getData() const { return m_data; }
        T &getData() { return m_data; }
        
        GeneralTreeNode *getParent() const { return _parent; }

        const std::vector<GeneralTreeNode<T> *> getChildren() const {
            std::vector<GeneralTreeNode<T> *> children {};
            std::transform(_children.begin(), _children.end(), std::back_inserter(children), [](const std::unique_ptr<GeneralTreeNode<T>> &child) { return child.get(); });
            return children;
        }
        
        GeneralTreeNode(const T &value, GeneralTreeNode *pParent);

        void AddChild(const GeneralTreeNodeComparator &comparator, bool bSorted, std::unique_ptr<GeneralTreeNode<T>> pChild);

        void RemoveChild(const GeneralTreeNodeComparator &comparator, const GeneralTreeNode *pChild);

        GeneralTreeNode *Reparent(const GeneralTreeNodeComparator &comparator, bool bSorted, GeneralTreeNode *pNewParent);

        int CompareTo(const GeneralTreeNodeComparator &comparator, const GeneralTreeNode<T> *cpOther);

    private:
        T m_data { };
        GeneralTreeNode *_parent = nullptr;
        std::vector<std::unique_ptr<GeneralTreeNode>> _children {};
    };

    template<typename T>
    GeneralTreeNode<T>::GeneralTreeNode(const T &value, GeneralTreeNode<T> *pParent)
    {
        m_data = value;
        _parent = pParent;
    }

    template<typename T>
    void GeneralTreeNode<T>::AddChild(const GeneralTreeNodeComparator &comparator, bool bSorted, std::unique_ptr<GeneralTreeNode> pChild)
    {
        pChild->_parent = this;
        if(bSorted) {
            Binary::BinaryInsert(_children, std::move(pChild), [comparator]
                    (const std::unique_ptr<GeneralTreeNode<T>> &cpLeft,
                     const std::unique_ptr<GeneralTreeNode<T>> &cpRight)
                { return comparator(cpLeft->getData(),cpRight->getData()); });
        }
        else _children.push_back(std::move(pChild));
    }

    template<typename T>
    void GeneralTreeNode<T>::RemoveChild(const GeneralTreeNodeComparator &comparator, const GeneralTreeNode *cpChild)
    {
        auto index = std::find_if(_children.begin(), _children.end(),[&comparator,cpChild](GeneralTreeNode *pNode){ return comparator(pNode->getData(),cpChild->getData()); });
        if (index != _children.end()) _children.erase(index);
        else throw DataStructureException("Couldn't find child {}", cpChild->getData());
    }

    template<typename T>
    GeneralTreeNode<T> *GeneralTreeNode<T>::Reparent(const GeneralTreeNodeComparator &comparator, bool bSorted, GeneralTreeNode<T> *pNewParent)
    {
        GeneralTreeNode *pOldParent = _parent;
        _parent = pNewParent;

        // remove ourself from the current parent
        pOldParent->RemoveChild(comparator, this);
        // add ourself to the new parent
        pNewParent->AddChild(comparator, bSorted, this);

        return pOldParent;
    }

    template<typename T>
    int GeneralTreeNode<T>::CompareTo(const GeneralTreeNodeComparator &comparator, const GeneralTreeNode<T> *cpOther)
    {
        return comparator(m_data,cpOther->m_data);
    }
}

#endif  // GENERALTREENODE_DOT_HPP
