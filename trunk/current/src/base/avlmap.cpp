/*
* This file is part of XTKLib project.
*
* Copyright(C) 2003-2005 Mario Casciaro (xshadow@email.it)
*
* Licensed under: 
*   - Apache License, Version 2.0 or
*   - GNU General Public License (GPL)
* You should have received at least one copy of them along with this program.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
* PURPOSE ARE DISCLAIMED.SEE YOUR CHOOSEN LICENSE FOR MORE DETAILS.
*/

/**
* @file avlmap.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "../../include/xtk/base/datastructures.h"
#include "../../include/xtk/base/exception.h"
#include "../../include/xtk/base/utilities.h"
#include "../../include/xtk/base/smartptr.h"
#include <stdlib.h>

namespace xtk
{
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xAvlMap::xAvlMap()
{
	m_pRootNode = NULL;
	nodeToDeleteWith2Child = NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xAvlMap::~xAvlMap()
{
	clear();
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xAvlMap::Node* xAvlMap::insert(xComparable* compKey,xObject* value,Node* root)
{
	if(root == NULL) 
		root = new Node(value,compKey);
	else
	{
		//insert in left child
		if(compKey->compareTo(*(root->key)) < 0) 
		{
			//recursive pass
			root->left = insert(compKey,value,root->left);
			
			//if after insertion the tree is not balanced
			if((getHeightOf(root->left) - getHeightOf(root->right)) == 2)
			{
				if(compKey->compareTo(*(root->left->key)) < 0)
					root = LLrotation(root); //LL
				else
					root = LRrotation(root); //LR
			}
		}
		//insert in right child
		else if(compKey->compareTo(*(root->key)) > 0) 
		{
			//recursive pass
			root->right = insert(compKey,value,root->right);
			
			//if after insertion the tree is not balanced
			if((getHeightOf(root->right) - getHeightOf(root->left)) == 2)
			{
				if(compKey->compareTo(*(root->right->key)) > 0)
					root = RRrotation(root); //RR
				else
					root = RLrotation(root); //RL
			}
		}
		//if duplicate key
		else
		{
			if(isOwner())
			{
				if((dynamic_cast<xComparable*>(root->data) != root->key) && (root->key != compKey))
					delete root->key;
				delete root->data;
			}
			else
			{
				if((dynamic_cast<xComparable*>(root->data) != root->key) && (root->key != compKey))
					delete root->key;
			}
			
			//substitute the value of current root
			root->data = value;
			root->key = compKey;
		}	
	}
	root->height = MAX(getHeightOf(root->left),getHeightOf(root->right)) + 1;
	
	return root;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xAvlMap::Node* xAvlMap::LLrotation(Node* root)
{
	Node* tmp = root->left;
	root->left = tmp->right;
	tmp->right = root;
	root->height = MAX(getHeightOf(root->left),getHeightOf(root->right)) + 1;
	tmp->height = MAX(getHeightOf(tmp->left),getHeightOf(root)) + 1;
	return tmp;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xAvlMap::Node* xAvlMap::LRrotation(Node* root)
{
	root->left = RRrotation(root->left);
	return LLrotation(root);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xAvlMap::Node* xAvlMap::RRrotation(Node* root)
{
	Node* tmp = root->right;
	root->right = tmp->left;
	tmp->left = root;
	root->height = MAX(getHeightOf(root->left),getHeightOf(root->right)) + 1;
	tmp->height = MAX(getHeightOf(tmp->left),getHeightOf(root)) + 1;
	return tmp;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xAvlMap::Node* xAvlMap::RLrotation(Node* root)
{
	root->right = LLrotation(root->right);
	return RRrotation(root);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xAvlMap::Node* xAvlMap::predecessor(xComparable* key)
{
	Node* pred = NULL;
	Node* root = m_pRootNode;
	while(root != NULL)
	{
		if(root->key->compareTo(*key) >= 0)
			root = root->left;
		else if(root->key->compareTo(*key) < 0)
		{
			pred = root;
			root = root->right;
      	}
	}
	return pred;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################s
xAvlMap::Node* xAvlMap::remove(xComparable* key,Node* root)
{
	if(root != NULL)
	{
		//begin search for the element to delete
		if(key->compareTo(*(root->key)) < 0)
		{
			root->left = remove(key,root->left);
		}
		else if(key->compareTo(*(root->key)) > 0)
		{
			root->right = remove(key,root->right);
		}
		else//we found it
		{
			//Case 1: node is a leaf, just delete it
			if(root->left == NULL && root->right == NULL)
			{
				if(isOwner())
				{
					if(dynamic_cast<xComparable*>(root->data) != root->key)
						delete root->key;
					delete root->data;
				}
				else
				{
					if(dynamic_cast<xComparable*>(root->data) != root->key)
						delete root->key;
				}
				delete root;
				
				root = NULL;
				return root;
			}
			//case 2.1 node with only left child
			else if(root->left != NULL && root->right == NULL)
			{
				Node* tmp = root->left;
				
				if(isOwner())
				{
					if(dynamic_cast<xComparable*>(root->data) != root->key)
						delete root->key;
					delete root->data;
				}
				else
				{
					if(dynamic_cast<xComparable*>(root->data) != root->key)
						delete root->key;
				}
				delete root;
				
				root = tmp;
			}
			//case 2.2 node with only right child
			else if(root->left == NULL && root->right != NULL)
			{
				Node* tmp = root->right;
				
				if(isOwner())
				{
					if(dynamic_cast<xComparable*>(root->data) != root->key)
						delete root->key;
					delete root->data;
				}
				else
				{
					if(dynamic_cast<xComparable*>(root->data) != root->key)
						delete root->key;
				}
				delete root;
				
				root = tmp;
			}
			//case 3 node with two child
			else
			{
				//return to root node and delete the predecessor
				nodeToDeleteWith2Child = root;
				if(root != m_pRootNode)
					return root;
			}
		}
		
		if((root == m_pRootNode) && nodeToDeleteWith2Child != NULL)
		{
			//if we are here we are in the root node and we should delete
			//the predecessor of the node
			
			//find predecessor
			Node* pred = predecessor(nodeToDeleteWith2Child->key);
			Node* temp = nodeToDeleteWith2Child;
			
			xObject* oldData = nodeToDeleteWith2Child->data;
			xComparable* oldKey = nodeToDeleteWith2Child->key;
			
			//copy precedessor data at place of root
			nodeToDeleteWith2Child->data = pred->data;
			xComparable* tmpKey = pred->key;
					
			//delete predecessor
			nodeToDeleteWith2Child = NULL;
			bool hasOwnership = isOwner();
			rescindOwnership();
			
			m_pRootNode = remove(pred->key,m_pRootNode);

			if(hasOwnership)
				giveOwnership();
				
			temp->key = tmpKey;
			
			if(isOwner())
			{
				if(dynamic_cast<xComparable*>(oldData) != oldKey)
					delete oldKey;
				delete oldData;
			}
			else
			{
				if(dynamic_cast<xComparable*>(oldData) != oldKey)
					delete oldKey;
			}
			
			return m_pRootNode;
		}

		//now balance if necessary
		int balFact = getHeightOf(root->left) - getHeightOf(root->right);
		//right imbalanced
		if(balFact == -2)
		{
			//root->right MUST be != NULL
			if(getHeightOf(root->right->right) > getHeightOf(root->right->left))
				root = RRrotation(root); //RR
			else
				root = RLrotation(root); //RL
		}
		//left imbalanced
		else if(balFact == 2)
		{
			//root->left MUST be != NULL
			if(getHeightOf(root->left->left) > getHeightOf(root->left->right))
				root = LLrotation(root); //LL
			else
				root = LRrotation(root); //LR
		}
		//else is balanced
		
		//now update height
		root->height = MAX(getHeightOf(root->left),getHeightOf(root->right)) + 1;
	}
	else //if key not found
	{
		//WHAT TO DO?
	}
	
	return root;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xAvlMap::getHeightOf(Node* node)
{
	if(node == NULL)
		return 0;
	else
		return node->height;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xAvlMap::size(Node* root)
{
	if(root != NULL)
	{
 		int subSize = 0;
 		subSize += size(root->left);
 		subSize += size(root->right);
 		return subSize + 1;
 	}
 	else
 		return 0;
}
 
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xAvlMap::put(xObject* key, xObject* value)
throw(xClassCastException)
{
	xComparable* compKey = dynamic_cast<xComparable*>(key);
	if(compKey == NULL)
		throw xClassCastException(_T("Key must implement xComparable"));
		
 	m_pRootNode = insert(compKey,value,m_pRootNode);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xAvlMap::clear(Node* root)
{
	if(root != NULL)
	{
		clear(root->left);
		clear(root->right);
		
		if(isOwner())
		{
			if(dynamic_cast<xComparable*>(root->data) != root->key)
				delete root->key;
			delete root->data;
		}
		else
		{
			if(dynamic_cast<xComparable*>(root->data) != root->key)
				delete root->key;
		}
		delete root;
	}
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xAvlMap::clear()
{
	clear(m_pRootNode);
	m_pRootNode = NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xAvlMap::containsKey(xObject& key)
throw(xClassCastException)
{
	xComparable* compKey = dynamic_cast<xComparable*>(&key);
	if(compKey == NULL)
		throw xClassCastException(_T("Key must implement xComparable"));
	
	Node* root = m_pRootNode;
	while(root != NULL)
	{
		if(compKey->compareTo(*(root->key)) < 0)
			root = root->left;
		else if(compKey->compareTo(*(root->key)) > 0) 
			root = root->right;
		else
			return true;
	}
	return false;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xAvlMap::containsValue(Node* root,xObject* value)
{
	if(value->equals(*(root->data)))
		return true;
	else
	{
		if(containsValue(root->left,value))
			return true;
		if(containsValue(root->right,value))
			return true;
	}
	return false;
}
	 
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xAvlMap::containsValue(xObject& value)
{
	return containsValue(m_pRootNode,&value);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xAvlMap::get(xObject& key)
throw(xClassCastException)
{
	xComparable* compKey = dynamic_cast<xComparable*>(&key);
	if(compKey == NULL)
		throw xClassCastException(_T("Key must implement xComparable"));
		
	Node* root = m_pRootNode;
	while(root != NULL)
	{
		if(compKey->compareTo(*(root->key)) < 0)
			root = root->left;
		else if(compKey->compareTo(*(root->key)) > 0) 
			root = root->right;
		else
			return *(root->data);
	}
	
	return xNullObject::getInstance();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xAvlMap::isEmpty()
{
	return m_pRootNode == NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xAvlMap::remove(xObject& key)
throw(xClassCastException)
{
	xComparable* compKey = dynamic_cast<xComparable*>(&key);
	if(compKey == NULL)
		throw xClassCastException(_T("Key must implement xComparable"));
		
	m_pRootNode = remove(compKey,m_pRootNode);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xAvlMap::size()
{
	return size(m_pRootNode);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#ifdef XTK_TEST

	bool xAvlMap::checkAVL(Node* root)
	{
		bool result = true;
		if(root != NULL)
		{
			if(abs(getHeightOf(root->left) - getHeightOf(root->right)) < 2)
			{
				if(root->left != NULL)
				{
					if(compare(root,root->left) < 0)
						return false;
					result = checkAVL(root->left);
				}
				else
					result = true;
				
				if(root->right != NULL)
				{
					if(compare(root,root->right) > 0)
						return false;
					result &= checkAVL(root->left);
				}
				else
					result &= true;
			}
			else
				result = false;
		}
			
		return result;
	}
	
	/**
	 * check if the tere is a valid AVL
	 */
	bool xAvlMap::checkAVL()
	{
		return checkAVL(m_pRootNode);
	}
	
	
	int xAvlMap::compare(Node* node1,Node* node2)
	{
		if(node1 == NULL)
			return -1;
		if(node2 == NULL)
			return 1;
		if(node1->key->compareTo(*(node2->key)) < 0)
			return -1;
		if(node1->key->compareTo(*(node2->key)) > 0)
			return 1;
		if(node1->key->compareTo(*(node2->key)) == 0)
			return 0;
			
		//never arrive here
		return 0;
	}
	
#endif
	
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xIterator* xAvlMap::iterator()
{
	return new xAvlMapIterator(*this);
}

	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xCollection* xAvlMap::values()
{
	xList* list = new xLinkedList();
	list->rescindOwnership();
	
	smartPtr<xIterator> iter = iterator();
	while(iter->hasNext())
	{
		list->add(&(iter->next()));	
	}
	return list;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xAvlMap::xAvlMapIterator::xAvlMapIterator(xAvlMap& map) 
: m_stack(0),m_map(map)
{
	m_stack.resize(map.size());
	m_position = -1;
	buildStack(map.m_pRootNode);
	m_position = -1;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xAvlMap::xAvlMapIterator::buildStack(Node* root)
{
	if(root != NULL)
	{
		buildStack(root->left);
		m_stack[++m_position] = root;
		buildStack(root->right);
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xAvlMap::xAvlMapIterator::hasNext()
{
	return (m_position + 1 < m_stack.size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xAvlMap::xAvlMapIterator::next()
throw(xNoSuchElementException)
{
	if(!hasNext())
		throw xNoSuchElementException();
	
	m_alreadyRemoved = false;
	return *(m_stack[++m_position]->data);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xAvlMap::xAvlMapIterator::remove()
throw(xIllegalStateException)
{
	if(m_position < 0 || m_alreadyRemoved)
		throw xIllegalStateException();
	
	m_alreadyRemoved = true;
	m_map.remove(*(m_stack[m_position]->key));
}	




}//end namespace

