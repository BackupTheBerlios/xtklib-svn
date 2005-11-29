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
* @file stacktrace.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/stacktrace.h"
#include "../../include/xtk/base/datastructures.h"

#ifdef XTK_OS_WINDOWS
	#include <windows.h>
#elif defined(XTK_OS_UNIX)
	#include <execinfo.h>
#endif



namespace xtk
{

#if defined(XTK_OS_WINDOWS) && defined(XTK_COMP_VISUAL_C)

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
typedef enum {
	SymNone = 0,
	SymCoff,
	SymCv,
	SymPdb,
	SymExport,
	SymDeferred,
	SymSym,
	SymDia,
	SymVirtual,
	NumSymTypes
} SYM_TYPE;
typedef struct _IMAGEHLP_LINE64 {
	DWORD                       SizeOfStruct;           // set to sizeof(IMAGEHLP_LINE64)
	PVOID                       Key;                    // internal
	DWORD                       LineNumber;             // line number in file
	PCHAR                       FileName;               // full filename
	DWORD64                     Address;                // first instruction of line
} IMAGEHLP_LINE64, *PIMAGEHLP_LINE64;
typedef struct _IMAGEHLP_MODULE64 {
	DWORD                       SizeOfStruct;           // set to sizeof(IMAGEHLP_MODULE64)
	DWORD64                     BaseOfImage;            // base load address of module
	DWORD                       ImageSize;              // virtual size of the loaded module
	DWORD                       TimeDateStamp;          // date/time stamp from pe header
	DWORD                       CheckSum;               // checksum from the pe header
	DWORD                       NumSyms;                // number of symbols in the symbol table
	SYM_TYPE                    SymType;                // type of symbols loaded
	CHAR                        ModuleName[32];         // module name
	CHAR                        ImageName[256];         // image name
	CHAR                        LoadedImageName[256];   // symbol file name
} IMAGEHLP_MODULE64, *PIMAGEHLP_MODULE64;
typedef struct _IMAGEHLP_SYMBOL64 {
	DWORD                       SizeOfStruct;           // set to sizeof(IMAGEHLP_SYMBOL64)
	DWORD64                     Address;                // virtual address including dll base address
	DWORD                       Size;                   // estimated size of symbol, can be zero
	DWORD                       Flags;                  // info about the symbols, see the SYMF defines
	DWORD                       MaxNameLength;          // maximum size of symbol name in 'Name'
	CHAR                        Name[1];                // symbol name (null terminated string)
} IMAGEHLP_SYMBOL64, *PIMAGEHLP_SYMBOL64;
typedef enum {
	AddrMode1616,
	AddrMode1632,
	AddrModeReal,
	AddrModeFlat
} ADDRESS_MODE;
typedef struct _tagADDRESS64 {
	DWORD64       Offset;
	WORD          Segment;
	ADDRESS_MODE  Mode;
} ADDRESS64, *LPADDRESS64;
typedef struct _KDHELP64 {
	DWORD64   Thread;
	DWORD   ThCallbackStack;
	DWORD   ThCallbackBStore;
	DWORD   NextCallback;
	DWORD   FramePointer;
	DWORD64   KiCallUserMode;
	DWORD64   KeUserCallbackDispatcher;
	DWORD64   SystemRangeStart;
	DWORD64  Reserved[8];
} KDHELP64, *PKDHELP64;
typedef struct _tagSTACKFRAME64 {
	ADDRESS64   AddrPC;               // program counter
	ADDRESS64   AddrReturn;           // return address
	ADDRESS64   AddrFrame;            // frame pointer
	ADDRESS64   AddrStack;            // stack pointer
	ADDRESS64   AddrBStore;           // backing store pointer
	PVOID       FuncTableEntry;       // pointer to pdata/fpo or NULL
	DWORD64     Params[4];            // possible arguments to the function
	BOOL        Far;                  // WOW far call
	BOOL        Virtual;              // is this a virtual frame?
	DWORD64     Reserved[3];
	KDHELP64    KdHelp;
} STACKFRAME64, *LPSTACKFRAME64,STACKFRAME;

#define SYMOPT_CASE_INSENSITIVE         0x00000001
#define SYMOPT_UNDNAME                  0x00000002
#define SYMOPT_DEFERRED_LOADS           0x00000004
#define SYMOPT_NO_CPP                   0x00000008
#define SYMOPT_LOAD_LINES               0x00000010
#define SYMOPT_OMAP_FIND_NEAREST        0x00000020
#define SYMOPT_LOAD_ANYTHING            0x00000040
#define SYMOPT_IGNORE_CVREC             0x00000080
#define SYMOPT_NO_UNQUALIFIED_LOADS     0x00000100
#define SYMOPT_FAIL_CRITICAL_ERRORS     0x00000200
#define SYMOPT_EXACT_SYMBOLS            0x00000400
#define SYMOPT_ALLOW_ABSOLUTE_SYMBOLS   0x00000800
#define SYMOPT_IGNORE_NT_SYMPATH        0x00001000
#define SYMOPT_INCLUDE_32BIT_MODULES    0x00002000
#define SYMOPT_PUBLICS_ONLY             0x00004000
#define SYMOPT_NO_PUBLICS               0x00008000
#define SYMOPT_AUTO_PUBLICS             0x00010000
#define SYMOPT_NO_IMAGE_SEARCH          0x00020000
#define SYMOPT_SECURE                   0x00040000
#define SYMOPT_DEBUG                    0x80000000
#define UNDNAME_COMPLETE                 (0x0000)  // Enable full undecoration
#define UNDNAME_NAME_ONLY                (0x1000)  // Crack only the name for primary declaration;


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################


HMODULE		g_hDbgHelpDll = NULL;

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

class xDbgHelp
{
public:
	typedef
		BOOL
		(WINAPI *PREAD_PROCESS_MEMORY_ROUTINE64)(
		HANDLE      hProcess,
		DWORD64     qwBaseAddress,
		PVOID       lpBuffer,
		DWORD       nSize,
		LPDWORD     lpNumberOfBytesRead
		);
	typedef
		PVOID
		(WINAPI *PFUNCTION_TABLE_ACCESS_ROUTINE64)(
		HANDLE  hProcess,
		DWORD64 AddrBase
		);
	typedef
		DWORD64
		(WINAPI *PGET_MODULE_BASE_ROUTINE64)(
		HANDLE  hProcess,
		DWORD64 Address
		);
	typedef
		DWORD64
		(WINAPI *PTRANSLATE_ADDRESS_ROUTINE64)(
		HANDLE    hProcess,
		HANDLE    hThread,
		LPADDRESS64 lpaddr
		);
	
	typedef BOOL (WINAPI *tSymCleanup)( IN HANDLE hProcess );
	static tSymCleanup SymCleanup;

	typedef PVOID (WINAPI *tSymFunctionTableAccess64)( HANDLE hProcess, DWORD64 AddrBase );
	static tSymFunctionTableAccess64 SymFunctionTableAccess64;

	typedef BOOL (WINAPI *tSymGetLineFromAddr64)( IN HANDLE hProcess, IN DWORD64 dwAddr,
		OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE64 Line );
	static tSymGetLineFromAddr64 SymGetLineFromAddr64;

	typedef DWORD64 (WINAPI *tSymGetModuleBase64)( IN HANDLE hProcess, IN DWORD64 dwAddr );
	static tSymGetModuleBase64 SymGetModuleBase64;

	typedef BOOL (WINAPI *tSymGetModuleInfo64)( IN HANDLE hProcess, IN DWORD64 dwAddr, OUT PIMAGEHLP_MODULE64 ModuleInfo );
	static tSymGetModuleInfo64 SymGetModuleInfo64;

	typedef DWORD (WINAPI *tSymGetOptions)( VOID );
	static tSymGetOptions SymGetOptions;

	typedef BOOL (WINAPI *tSymGetSymFromAddr64)( IN HANDLE hProcess, IN DWORD64 dwAddr,
		OUT PDWORD64 pdwDisplacement, OUT PIMAGEHLP_SYMBOL64 Symbol );
	static tSymGetSymFromAddr64 SymGetSymFromAddr64;

	typedef BOOL (WINAPI *tSymInitialize)( IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess );
	static tSymInitialize SymInitialize;

	typedef DWORD64 (WINAPI *tSymLoadModule64)( IN HANDLE hProcess, IN HANDLE hFile,
		IN PSTR ImageName, IN PSTR ModuleName, IN DWORD64 BaseOfDll, IN DWORD SizeOfDll );
	static tSymLoadModule64 SymLoadModule64;

	typedef DWORD (WINAPI *tSymSetOptions)( IN DWORD SymOptions );
	static tSymSetOptions SymSetOptions;

	typedef BOOL (WINAPI *tStackWalk64)(
		DWORD MachineType, 
		HANDLE hProcess,
		HANDLE hThread, 
		LPSTACKFRAME64 StackFrame, 
		PVOID ContextRecord,
		PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
		PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
		PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
		PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress );
	static tStackWalk64 StackWalk64;

	typedef DWORD (WINAPI *tUnDecorateSymbolName)( PCSTR DecoratedName, PSTR UnDecoratedName,
		DWORD UndecoratedLength, DWORD Flags );
	static tUnDecorateSymbolName UnDecorateSymbolName;
	

	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	static bool init()
	{
		if (g_hDbgHelpDll != NULL)  
			return true;
		else
		{
			// if not already loaded, try to load
			g_hDbgHelpDll = LoadLibrary( _T("dbghelp.dll") );
			if (g_hDbgHelpDll == NULL)
				return FALSE;
		}
			
		//now bind functions
		SymInitialize = (tSymInitialize) GetProcAddress(g_hDbgHelpDll, "SymInitialize" );
		SymCleanup = (tSymCleanup) GetProcAddress(g_hDbgHelpDll, "SymCleanup" );
		
		StackWalk64 = (tStackWalk64) GetProcAddress(g_hDbgHelpDll, "StackWalk64" );
		SymGetModuleInfo64 = (tSymGetModuleInfo64) GetProcAddress(g_hDbgHelpDll, "SymGetModuleInfo64" );
		SymSetOptions = (tSymSetOptions) GetProcAddress(g_hDbgHelpDll, "SymSetOptions" );
		SymGetOptions = (tSymGetOptions) GetProcAddress(g_hDbgHelpDll, "SymGetOptions" );

		SymFunctionTableAccess64 = (tSymFunctionTableAccess64) GetProcAddress(g_hDbgHelpDll, "SymFunctionTableAccess64" );
		SymGetLineFromAddr64 = (tSymGetLineFromAddr64) GetProcAddress(g_hDbgHelpDll, "SymGetLineFromAddr64" );
		SymGetModuleBase64 = (tSymGetModuleBase64) GetProcAddress(g_hDbgHelpDll, "SymGetModuleBase64" );
		SymGetSymFromAddr64 = (tSymGetSymFromAddr64) GetProcAddress(g_hDbgHelpDll, "SymGetSymFromAddr64" );
		UnDecorateSymbolName = (tUnDecorateSymbolName) GetProcAddress(g_hDbgHelpDll, "UnDecorateSymbolName" );
		SymLoadModule64 = (tSymLoadModule64) GetProcAddress(g_hDbgHelpDll, "SymLoadModule64" );

		if (SymInitialize == NULL || SymCleanup == NULL || StackWalk64 == NULL || SymGetOptions == NULL ||
			SymSetOptions == NULL || SymFunctionTableAccess64 == NULL || SymGetLineFromAddr64 == NULL || 
			SymGetModuleBase64 == NULL || SymGetSymFromAddr64 == NULL || UnDecorateSymbolName == NULL || 
			SymLoadModule64 == NULL || SymGetModuleInfo64 == NULL)
		{
			FreeLibrary(g_hDbgHelpDll);
			g_hDbgHelpDll = NULL;
			return false;
		}

		return true;
	}

};


xDbgHelp::tSymCleanup xDbgHelp::SymCleanup;
xDbgHelp::tSymFunctionTableAccess64 xDbgHelp::SymFunctionTableAccess64;
xDbgHelp::tSymGetLineFromAddr64 xDbgHelp::SymGetLineFromAddr64;
xDbgHelp::tSymGetModuleBase64 xDbgHelp::SymGetModuleBase64;
xDbgHelp::tSymGetModuleInfo64 xDbgHelp::SymGetModuleInfo64;
xDbgHelp::tSymGetOptions xDbgHelp::SymGetOptions;
xDbgHelp::tSymGetSymFromAddr64 xDbgHelp::SymGetSymFromAddr64;
xDbgHelp::tSymInitialize xDbgHelp::SymInitialize;
xDbgHelp::tSymLoadModule64 xDbgHelp::SymLoadModule64;
xDbgHelp::tSymSetOptions xDbgHelp::SymSetOptions;
xDbgHelp::tStackWalk64 xDbgHelp::StackWalk64;
xDbgHelp::tUnDecorateSymbolName xDbgHelp::UnDecorateSymbolName;

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xStackFrameInternal
{
public:

	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	static void getCurrentStackTrace(xArray<xStackFrame*>* a,size_t skip)
	{
	
		//get current context by forcing an exception
		__try
		{
			throw 0;
		}
		__except(walkFrom((EXCEPTION_POINTERS *)GetExceptionInformation(),skip+5,a) 
			? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_EXECUTE_HANDLER )
		{}
		
	}
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	static xArray<xStackFrame*> walkFrom(const CONTEXT *pCtx, size_t skip)
	{
		xArrayList	frames;
		frames.rescindOwnership();
		
		if (!xDbgHelp::init())
		{
			return xArray<xStackFrame*>(0);
		}

		HANDLE hProcess = ::GetCurrentProcess();
		//DWORD t = GetLastError();
		if( !xDbgHelp::SymInitialize
							(
							hProcess,
							NULL,   // use default symbol search path
							TRUE    // load symbols for all loaded modules
							) 
		  )
		{
			return xArray<xStackFrame*>(0);
		}

		CONTEXT ctx = *pCtx; //will be modified by StackWalk()

		DWORD dwMachineType;
		
		//initialize the initial frame
		STACKFRAME sf;
		memset(&sf,0,sizeof(STACKFRAME));

		#ifdef _M_IX86
			sf.AddrPC.Offset       = ctx.Eip;
			sf.AddrPC.Mode         = AddrModeFlat;
			sf.AddrStack.Offset    = ctx.Esp;
			sf.AddrStack.Mode      = AddrModeFlat;
			sf.AddrFrame.Offset    = ctx.Ebp;
			sf.AddrFrame.Mode      = AddrModeFlat;
			
			dwMachineType = IMAGE_FILE_MACHINE_I386;
		#elif _M_X64
			sf.AddrPC.Offset = ctx.Rip;
			sf.AddrPC.Mode = AddrModeFlat;
			sf.AddrFrame.Offset = ctx.Rsp;
			sf.AddrFrame.Mode = AddrModeFlat;
			sf.AddrStack.Offset = ctx.Rsp;
			sf.AddrStack.Mode = AddrModeFlat;
			
			dwMachineType = IMAGE_FILE_MACHINE_AMD64;
		#elif _M_IA64
			sf.AddrPC.Offset = ctx.StIIP;
			sf.AddrPC.Mode = AddrModeFlat;
			sf.AddrFrame.Offset = ctx.IntSp;
			sf.AddrFrame.Mode = AddrModeFlat;
			sf.AddrBStore.Offset = ctx.RsBSP;
			sf.AddrBStore.Mode = AddrModeFlat;
			sf.AddrStack.Offset = ctx.IntSp;
			sf.AddrStack.Mode = AddrModeFlat;
				
			dwMachineType = IMAGE_FILE_MACHINE_IA64;
		#else
			#error "Cannot use xStackTrace on your machine"
		#endif // _M_IX86

		//iterate stack frames
		for(size_t nLevel = 0;;nLevel++)
		{
			// get the next stack frame
			if ( !xDbgHelp::StackWalk64
				(
				dwMachineType,
				hProcess,
				::GetCurrentThread(),
				&sf,
				&ctx,
				NULL,       // read memory function (default)
				xDbgHelp::SymFunctionTableAccess64,
				xDbgHelp::SymGetModuleBase64,
				NULL        // address translator for 16 bit
				)
			  )
			{
				break;
			}

			if( nLevel >= skip )
				frames.add(getStackFrame(&sf,nLevel - skip));
		}

		// this results in crashes inside ntdll.dll when called from
		// exception handler ...
		#if 0
			xDbgHelp::SymCleanup(hProcess);
		#endif
		
		return frames.toArray().castTo<xStackFrame*>();
	}
	
	
	
private:
	enum{ STACK_MAX_NAMELEN = 512};


	static xStackFrame* getStackFrame(STACKFRAME* frame,size_t level)
	{
		xStackFrame* stackframe = new xStackFrame();
		stackframe->m_level = (int) level;
		stackframe->m_address = (void*) frame->AddrPC.Offset;
		
		HANDLE hProcess = ::GetCurrentProcess();
		
		IMAGEHLP_SYMBOL64 *pSym = (IMAGEHLP_SYMBOL64 *) malloc(sizeof(IMAGEHLP_SYMBOL64) + STACK_MAX_NAMELEN);
		memset(pSym, 0, sizeof(IMAGEHLP_SYMBOL64) + STACK_MAX_NAMELEN);
		pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
		pSym->MaxNameLength = STACK_MAX_NAMELEN;
		
		
		//extract name
		if(
			xDbgHelp::SymGetSymFromAddr64(hProcess, frame->AddrPC.Offset,NULL, pSym) 
			!= FALSE
		  )
		{
			char tmp[STACK_MAX_NAMELEN];
			// UnDecorateSymbolName()
			xDbgHelp::UnDecorateSymbolName(pSym->Name, tmp, STACK_MAX_NAMELEN, UNDNAME_COMPLETE );
			
			#ifndef XTK_UNICODE
				stackframe->m_name = tmp;
			#else
				stackframe->m_name = xString(tmp,xCharset::CS_SYSTEM);
			#endif
		}
		free(pSym);
		
		//extract line number and file
		/*
		IMAGEHLP_LINE64 line;
		memset(&line, 0, sizeof(line));
		line.SizeOfStruct = sizeof(line);
		if (xDbgHelp::SymGetLineFromAddr64(hProcess, frame->AddrPC.Offset, NULL, &line) != FALSE)
		{
			//stackframe->m_lineNumber = line.LineNumber;
			char* tmp = line.FileName;
			#ifndef XTK_UNICODE
				//stackframe->m_fileName = line.FileName;
			#else
				//stackframe->m_fileName = xString(line.FileName,xCharset::CS_SYSTEM);
			#endif
		}
		DWORD t = GetLastError();
		*/
		
		IMAGEHLP_MODULE64 module;
		memset(&module, 0, sizeof(module));
		module.SizeOfStruct = sizeof(module);
		//extract module name
		if (xDbgHelp::SymGetModuleInfo64(hProcess, frame->AddrPC.Offset, &module ) != FALSE)
		{ 
			#ifndef XTK_UNICODE
				stackframe->m_module = module.ImageName;
			#else
				stackframe->m_module = xString(module.ImageName,xCharset::CS_SYSTEM);
			#endif
		}
		
		
		return stackframe;
	}

	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	static bool walkFrom(const _EXCEPTION_POINTERS *ep, size_t skip,xArray<xStackFrame*>* ret)
	{
		*ret = walkFrom(ep->ContextRecord, skip);
		return true;
	}
	

};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArray<xStackFrame*> xStackTrace::getCurrentStackTrace(size_t skip)
{
	xArray<xStackFrame*> ret;
	xStackFrameInternal::getCurrentStackTrace(&ret,skip);
	return ret;
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
#elif defined(XTK_OS_UNIX)



xArray<xStackFrame*> xStackTrace::getCurrentStackTrace(size_t skip)
{
	xArray<xStackFrame*> ret;
	void* addresses[256];

	int depth = ::backtrace(addresses, sizeof(addresses));
	if ( !depth )
		return ret;

	ret.resize(depth - (int)skip);
	char** symbols = ::backtrace_symbols(addresses, depth);
	int n = skip + 1;
	int i = 0;
	for(; n < depth; n++,i++ )
	{
		xStackFrame* stackf = new xStackFrame();
		stackf->m_level = i;
		stackf->m_address = addresses[n];
		
		#ifndef XTK_UNICODE
			stackf->m_name = symbols[n];
		#else
			stackf->m_name = xString(symbols[n],xCharset::CS_SYSTEM);
		#endif
		
		ret[i] = stackf;
	}
	
	free(symbols);
	return ret;
}



#else //no supported system
	

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

xArray<xStackFrame*> xStackTrace::getCurrentStackTrace(size_t skip)
{
	return xArray<xStackFrame*>();
}
	
#endif
	
}//namespace
