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

#ifndef GENERALTREE_DOT_HPP
#define GENERALTREE_DOT_HPP

#include <vector>
#ifndef GENERALTREENODE_DOT_HPP
    #include "data-structures/tree/GeneralTreeNode.hpp"
#endif
#ifndef BINARYLIST_DOT_HPP
    #include "data-structures/binary/BinaryList.hpp"
#endif
#ifndef DATASTRUCTURES_DATASTRUCTUREEXCEPTION_DOT_HPP
    #include "data-structures/DataStructureException.hpp"
#endif

namespace DataStructures::Tree
{
    template<typename T>
    class GeneralTree
    {
    public:
        std::vector<GeneralTreeNode<T> *> getRoots() const;
        GeneralTree(const GeneralTreeNode<T>::GeneralTreeNodeComparator &comparator, bool bSorted = false):
            m_comparator(comparator),
            m_bSorted(bSorted)
        {
        }

        GeneralTreeNode<T> *AddRoot(const T &value);
        GeneralTreeNode<T> *AddRoot(std::unique_ptr<GeneralTreeNode<T>> pNewRoot);

        GeneralTreeNode<T> *AddChild(GeneralTreeNode<T> *pParent, const T &value);

        void AddChild(GeneralTreeNode<T> *pParent, std::unique_ptr<GeneralTreeNode<T>> pChild);

        void Reparent(GeneralTreeNode<T> *pNewParent, GeneralTreeNode<T> *pChild);

        bool IsRoot(const GeneralTreeNode<T> *cpNode) const
        {
            return cpNode->getParent() == _sentinel;
        }

        const GeneralTreeNode<T> *getSentinel() const { return _sentinel.get(); }

    private:
        const GeneralTreeNode<T>::GeneralTreeNodeComparator &m_comparator;
        bool m_bSorted = false;
        std::unique_ptr<GeneralTreeNode<T>> _sentinel = std::make_unique<GeneralTreeNode<T>>(T{},nullptr);
        std::vector<std::unique_ptr<GeneralTreeNode<T>>> _roots = std::vector<std::unique_ptr<GeneralTreeNode<T>>>();
    };

    template<typename T>
    std::vector<GeneralTreeNode<T> *> GeneralTree<T>::getRoots() const
    {
        std::vector<GeneralTreeNode<T> *> roots {};
        roots.reserve(_roots.size());
        std::transform(_roots.begin(), _roots.end(), std::back_inserter(roots), [](const std::unique_ptr<GeneralTreeNode<T>> &root) { return root.get(); });
        return roots;
    }

    template<typename T>
    GeneralTreeNode<T> *GeneralTree<T>::AddRoot(const T& value)
    {
        std::unique_ptr<GeneralTreeNode<T>> pRoot = std::make_unique<GeneralTreeNode<T>>(value, _sentinel.get());
        return AddRoot(std::move(pRoot));
    }

    template<typename T>
    GeneralTreeNode<T> *GeneralTree<T>::AddRoot(std::unique_ptr<GeneralTreeNode<T>> pNewRoot)
    {
        // this is what we returned, it gets moved btwn here and there but it's what we want to return.
        GeneralTreeNode<T> *pResult = pNewRoot.get();
        if(pNewRoot->getParent() != _sentinel.get()) throw DataStructureException("AddRoot called on node that has a parent!");
        if(m_bSorted) {
            Binary::BinaryInsert(_roots, std::move(pNewRoot), [this]
                    (const std::unique_ptr<GeneralTreeNode<T>> &cpLeft,
                     const std::unique_ptr<GeneralTreeNode<T>> &cpRight)
                { return this->m_comparator(cpLeft->getData(),cpRight.get()->getData()); });
        }
        else {
            _roots.push_back(std::move(pNewRoot));
        }
        // hurp: can't use pNewRoot after it's std::move'd.
        return pResult;
    }

    template<typename T>
    GeneralTreeNode<T> *GeneralTree<T>::AddChild(GeneralTreeNode<T> *pParent, const T &value)
    {
        std::unique_ptr<GeneralTreeNode<T>> pChild = std::make_unique<GeneralTreeNode<T>>(value, pParent);
        pParent->AddChild(m_comparator, m_bSorted, std::move(pChild));
        return pChild.get();
    }

    template<typename T>
    void GeneralTree<T>::AddChild(GeneralTreeNode<T> *pParent, std::unique_ptr<GeneralTreeNode<T>> pChild)
    {
        // if this child already has a parent
        if(pChild-> getParent() != _sentinel) {
            // noop
            if(pParent == pChild->getParent()) return;

            // ensure the consistency of the tree
            pChild->Reparent(m_comparator, m_bSorted, pParent);
        }

        // i.e., reparent as a root
        if(pParent == _sentinel) {
            if(m_bSorted) {
                Binary::BinaryInsert(_roots, std::move(pChild), [this]
                        (const std::unique_ptr<GeneralTreeNode<T>> &cpLeft,
                         const std::unique_ptr<GeneralTreeNode<T>> &cpRight)
                    { return this->m_comparator(cpLeft->getData(),cpRight.get()->getData()); });
            }
            else _roots.push_back(std::move(pChild));
        }

        pParent->AddChild(m_comparator, m_bSorted, pChild);
    }

    template<typename T>
    void GeneralTree<T>::Reparent(GeneralTreeNode<T> *pNewParent, GeneralTreeNode<T> *pChild)
    {
        if (pChild->getParent() == _sentinel)
        {
            // noop
            if(pNewParent == _sentinel) return;

            auto it = std::find_if(_roots.begin(),_roots.end(), [this,pChild](std::unique_ptr<GeneralTreeNode<T>> pNode) { return !this->m_comparator(pNode->getData(),pChild->getData()); });
            if(it == _roots.end()) throw DataStructureException("Child is not a root.");
            std::unique_ptr<GeneralTreeNode<T>> pUniqueChild = *it;
            _roots.erase(it);

            pNewParent->AddChild(m_comparator, m_bSorted, pUniqueChild);

            return;
        }

        if(pNewParent == _sentinel) {
            // noop again
            if(pChild->getParent() == _sentinel) return;

            auto it = std::find_if(_roots.begin(),_roots.end(), [this,pChild](std::unique_ptr<GeneralTreeNode<T>> pNode) { return !this->m_comparator(pNode->getData(),pChild->getData()); });
            if(it != _roots.end()) throw DataStructureException("Internal error - child is already a root.");

            pChild->getParent()->RemoveChild(m_comparator, m_bSorted, pChild);
            AddRoot(pChild);
        }

        pChild->getParent()->RemoveChild(m_comparator, m_bSorted, pChild);
        pNewParent->AddChild(m_comparator, m_bSorted, pChild);
    }
}

#endif  /* GENERALTREE_DOT_HPP */
