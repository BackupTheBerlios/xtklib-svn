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
* @file memory.h
* @author Mario Casciaro (xshadow@email.it)
*/


#ifndef XTK_MEMORY_H
#define XTK_MEMORY_H

#ifndef XTK_GLOBALS_H
#	error "Must include ../globals.h"
#endif

#ifdef XTK_OS_WINDOWS
    #include <windows.h>
    #include <process.h>
    typedef HANDLE xgc_mutex_id;

#else //if UNIX
    #include <pthread.h>
	#include <errno.h>
    typedef pthread_mutex_t xgc_mutex_id;

#endif


namespace xtk
{

#ifdef XTK_USE_MEMCHECK
	/**
	 * 
	 */
	class xAvlMemTrace
	{
	protected:
	
		#ifdef XTK_USE_MULTITHREAD
			#ifdef XTK_OS_WINDOWS
			    HANDLE m_mem_check_mutex_id;
			#else //if UNIX
			    pthread_mutex_t m_mem_check_mutex_id;
			#endif
			
			bool m_bMemCheckMutexCreated;
			
			
			void createMemCheckMutex();
			void syncMemCheckStart();
			void syncMemCheckEnd();
		#endif //XTK_USE_MULTITHREAD
	
	
		struct Node
		{
			Node*			left;
			Node*			right;
			void*			key;
			const char*		file;
			int 			line;
			size_t			size;
			int				height;
			
			Node(const char* file,int line,size_t size,void* key,Node* left = NULL,
					Node* right = NULL)
			{
				this->left = left;
				this->right = right;
				this->key = key;
				this->file = file;
				this->line = line;
				this->size = size;
				this->height = 0;
			}
		};
		
		Node*		m_pRootNode;
		Node* 		nodeToDeleteWith2Child;
		
		/*
		 * Recursive insert. Return the new root
		 */
		Node* insert(void* compKey,const char* file, int line,size_t size,Node* root);
		

		Node* LLrotation(Node* root);
		Node* LRrotation(Node* root);
		Node* RRrotation(Node* root);
		Node* RLrotation(Node* root);
		
		/*
		 * Find the maximum between of all nodes < of "node"
		 */
		Node* predecessor(void* key);
		
		/*
		 * return new root
		 */
		Node* remove(void* key,Node* root);
		
		int getHeightOf(Node* node);
	public:
		xAvlMemTrace();
		
		/**
	     * Associates the specified value with the specified key in this map.
	     */
		void put(void* key, const char* file,int line,size_t size)
		{
			if(!s_startTrackMemory)
				return;

			#ifdef XTK_USE_MULTITHREAD
				syncMemCheckStart();
			#endif
			
			m_pRootNode = insert(key,file,line,size,m_pRootNode);

			#ifdef XTK_USE_MULTITHREAD
				syncMemCheckEnd();
			#endif
		}
	    
	    /**
		 * Removes the mapping for this key from this map if it is present 
		 */
		void remove(void* key)
		{
			if(!s_startTrackMemory)
				return;
				
			#ifdef XTK_USE_MULTITHREAD
				syncMemCheckStart();
			#endif
				
			m_pRootNode = remove(key,m_pRootNode);
			
			#ifdef XTK_USE_MULTITHREAD
				syncMemCheckEnd();
			#endif
		}
		
	    /**
	     * Returns true if this map contains no key-value mappings.
	     */
		bool isEmpty()
		{
			return m_pRootNode == NULL;
			
		}
		
		static bool s_startTrackMemory;
		
		friend void xDumpMemoryLeaks(xAvlMemTrace::Node* root);
		friend void xDumpMemoryLeaks();
	};
#endif

	//this are defined to made safe the use with dll
	XTKAPI void* xtk_malloc(size_t sz);
	XTKAPI void xtk_free(void* ptr);
}//namespace




#endif /*XTK_MEMORY_H*/
