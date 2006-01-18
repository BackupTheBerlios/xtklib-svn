/*
* This file is part of XTKLib project.
*
* Copyright(C) 2003-2006 Mario Casciaro xshadow[AT]email(DOT)it
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
* @file memory.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/globals.h"

namespace xtk
{

#ifdef XTK_USE_MEMCHECK
	xAvlMemTrace 	g_memoryTrace;
	bool xAvlMemTrace::s_startTrackMemory = false;


	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	void xAvlMemTrace::createMemCheckMutex()
	{
	#ifdef XTK_OS_WINDOWS
	
		m_mem_check_mutex_id = ::CreateMutex( NULL, TRUE, NULL );
		if(m_mem_check_mutex_id == NULL)
		{
			printf("Cannot create mutex for memcheck");
		}    
		else
		{
			//windows mutex are locked by default
			::ReleaseMutex((HANDLE)m_mem_check_mutex_id);
		}    
	
	#elif defined(XTK_OS_UNIX)
	
		int res;
		pthread_mutexattr_t attr;
		::pthread_mutexattr_init(&attr);
		::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		res = ::pthread_mutex_init(&m_mem_check_mutex_id, &attr);
		::pthread_mutexattr_destroy(&attr);
		if(res != 0)
			printf("Cannot create mutex for memcheck");
	#endif
		
		m_bMemCheckMutexCreated = true;
	}
	
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	void xAvlMemTrace::syncMemCheckStart()
	{
		if(!m_bMemCheckMutexCreated)
			createMemCheckMutex();
	
		#ifdef XTK_OS_WINDOWS
			::WaitForSingleObject((HANDLE)m_mem_check_mutex_id, INFINITE);
		#else //POSIX systems
			::pthread_mutex_lock(&m_mem_check_mutex_id);
		#endif
	}
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	void xAvlMemTrace::syncMemCheckEnd()
	{
		if(m_bMemCheckMutexCreated)
		{
			#ifdef XTK_OS_WINDOWS
				::ReleaseMutex((HANDLE)m_mem_check_mutex_id);
			#else //POSIX systems
				::pthread_mutex_unlock(&m_mem_check_mutex_id);
			#endif
		}
	}
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	xAvlMemTrace::Node* xAvlMemTrace::insert(void* compKey,const char* file, int line,size_t size,Node* root)
	{
		if(root == NULL) 
			root = new Node(file,line,size,compKey);
		else
		{
			//insert in left child
			if(compKey < root->key) 
			{
				//recursive pass
				root->left = insert(compKey,file,line,size,root->left);
				
				//if after insertion the tree is not balanced
				if((getHeightOf(root->left) - getHeightOf(root->right)) == 2)
				{
					if(compKey < root->left->key)
						root = LLrotation(root); //LL
					else
						root = LRrotation(root); //LR
				}
			}
			//insert in right child
			else if(compKey > root->key) 
			{
				//recursive pass
				root->right = insert(compKey,file,line,size,root->right);
				
				//if after insertion the tree is not balanced
				if((getHeightOf(root->right) - getHeightOf(root->left)) == 2)
				{
					if(compKey > root->right->key)
						root = RRrotation(root); //RR
					else
						root = RLrotation(root); //RL
				}
			}
			//if duplicate key
			else
			{	
				//substitute the value of current root
				root->file = file;
				root->line = line;
				root->size = size;
				root->key = compKey;
			}	
		}
		root->height = MAX(getHeightOf(root->left),getHeightOf(root->right)) + 1;
		
		return root;
	}
	
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	xAvlMemTrace::Node* xAvlMemTrace::LLrotation(Node* root)
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
	xAvlMemTrace::Node* xAvlMemTrace::LRrotation(Node* root)
	{
		root->left = RRrotation(root->left);
		return LLrotation(root);
	}
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	xAvlMemTrace::Node* xAvlMemTrace::RRrotation(Node* root)
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
	xAvlMemTrace::Node* xAvlMemTrace::RLrotation(Node* root)
	{
		root->right = LLrotation(root->right);
		return RRrotation(root);
	}
			
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	xAvlMemTrace::Node* xAvlMemTrace::predecessor(void* key)
	{
		Node* pred = NULL;
		Node* root = m_pRootNode;
		while(root != NULL)
		{
			if(root->key >= key)
				root = root->left;
			else if(root->key < key)
			{
				pred = root;
				root = root->right;
	      	}
		}
		return pred;
	}
			
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	xAvlMemTrace::Node* xAvlMemTrace::remove(void* key,Node* root)
	{
		if(root != NULL)
		{
			//begin search for the element to delete
			if(key < root->key)
			{
				root->left = remove(key,root->left);
			}
			else if(key > root->key)
			{
				root->right = remove(key,root->right);
			}
			else//we found it
			{
				//Case 1: node is a leaf, just delete it
				if(root->left == NULL && root->right == NULL)
				{
					delete root;
					root = NULL;
					return root;
				}
				//case 2.1 node with only left child
				else if(root->left != NULL && root->right == NULL)
				{
					Node* tmp = root->left;
					delete root;
					
					root = tmp;
				}
				//case 2.2 node with only right child
				else if(root->left == NULL && root->right != NULL)
				{
					Node* tmp = root->right;
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
				
				//copy predecessor data at place of root
				nodeToDeleteWith2Child->file = pred->file;
				nodeToDeleteWith2Child->line = pred->line;
				nodeToDeleteWith2Child->size = pred->size;
				void* tmpKey = pred->key;
						
				//delete predecessor
				nodeToDeleteWith2Child = NULL;
				m_pRootNode = remove(pred->key,m_pRootNode);	
				temp->key = tmpKey;
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
	int xAvlMemTrace::getHeightOf(Node* node)
	{
		if(node == NULL)
			return 0;
		else
			return node->height;	
	}
	
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	xAvlMemTrace::xAvlMemTrace()
	{
		#ifdef XTK_USE_MULTITHREAD
			syncMemCheckStart();
		#endif
		
		m_pRootNode = NULL;
		nodeToDeleteWith2Child = NULL;
		
		#ifdef XTK_USE_MULTITHREAD
			syncMemCheckEnd();
		#endif
	}

	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	bool xDumpMemoryLeaks(xAvlMemTrace::Node* root)
	{
		if(root != NULL)
		{
			xDumpMemoryLeaks(root->left);
			xDumpMemoryLeaks(root->right);

			printf("\n\nMemory Leak detected:");
			printf("\nFile: %s",root->file);
			printf("\nLine: %d",root->line);
			printf("\nSize: %ul \n",(unsigned int) root->size);
			return true;
		}
		return false;
	}

	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	void xDumpMemoryLeaks()
	{
		if(!xAvlMemTrace::s_startTrackMemory)
			printf("Memory tracking not started, use xStartMemoryTracking()");

		if(xDumpMemoryLeaks(g_memoryTrace.m_pRootNode))
		{
			printf("Press \"enter\" key to continue...");
			fgetc(stdin);
		}
		//xAvlMemTrace::s_startTrackMemory = false;
	}
#endif

	//this are defined to made safe the use with dll
	void* xtk_malloc(size_t sz){return ::malloc(sz);}
	void xtk_free(void* ptr){::free(ptr);}
}//namespace

