// Copyright(C) 2003-2005 Mario Casciaro (xshadow@email.it)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
* @file process.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/process.h"
#include "../../include/xtk/base/typewrappers.h"

#ifdef XTK_OS_WINDOWS
	#include <Windows.h>
#elif defined(XTK_OS_UNIX)
	#include <unistd.h>
	#include <sys/wait.h>
	#include <signal.h>
#endif


namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xPipeInputStream : public xInputStream
{
private:
	#ifdef XTK_OS_WINDOWS
		HANDLE	m_hPipeIn;
	#elif defined(XTK_OS_UNIX)
		int		m_fdPipeIn;
	#endif
	bool		m_isClosed;
	
public:
	#ifdef XTK_OS_WINDOWS
		xPipeInputStream(HANDLE hPipeIn){m_hPipeIn = hPipeIn;m_isClosed = false;}
	#elif defined(XTK_OS_UNIX)
		xPipeInputStream(int fdPipeIn){m_fdPipeIn = fdPipeIn;m_isClosed = false;}
	#endif
	
	virtual ~xPipeInputStream(){if(!isClosed()) close();}


	virtual int available();
	virtual int read() throw(xIOException);
	virtual int read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	virtual void close() throw(xIOException);
	virtual bool isClosed(){return m_isClosed;}
};


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xPipeOutputStream : public xOutputStream
{
private:
	#ifdef XTK_OS_WINDOWS
		HANDLE	m_hPipeOut;
	#elif defined(XTK_OS_UNIX)
		int		m_fdPipeOut;
	#endif
	bool		m_isClosed;

public:
	#ifdef XTK_OS_WINDOWS
		xPipeOutputStream(HANDLE hPipeOut){m_hPipeOut = hPipeOut;m_isClosed = false;}
	#elif defined(XTK_OS_UNIX)
		xPipeOutputStream(int fdPipeOut){m_fdPipeOut = fdPipeOut;m_isClosed = false;}
	#endif

	virtual ~xPipeOutputStream(){if(!isClosed())close();}

	virtual void close() throw(xIOException);
	virtual bool isClosed(){return m_isClosed;}
	virtual void write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	virtual void write(int b) throw(xIOException);
};
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xPipeInputStream::available()
{
	if (isClosed())
		return false;
		
	#ifdef XTK_OS_WINDOWS
		DWORD nAvailable;

		//it works with anon pipes as well
		DWORD rc = ::PeekNamedPipe
			(
			m_hPipeIn,     // handle
			NULL, 0,      // ptr to buffer and its size
			NULL,         // [out] bytes read
			&nAvailable,  // [out] bytes available
			NULL          // [out] bytes left
			);

		if (!rc)
			return 0;

		return (int)nAvailable;
		
	#elif defined(XTK_OS_UNIX)
		// check if there is any input available
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(m_fdPipeIn, &readfds);
		if(::select(m_fdPipeIn + 1, &readfds, NULL, NULL, &tv) != 1)
			return 0;
		
		return 1;
		
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xPipeInputStream::read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException)
{
	if(isClosed())
		throw xIOException(_T("Cannot read data, pipe closed"));
	if(len <= 0 || off+len > a.size() || off < 0)
		throw xIndexOutOfBoundsException();
		
	#ifdef XTK_OS_WINDOWS
		DWORD bytesRead;
		if ( !::ReadFile(m_hPipeIn, a.getRawData() + off, len, &bytesRead, NULL) )
		{
			if(::GetLastError() == ERROR_BROKEN_PIPE)
				return xEOF;
			else
				throw xIOException(_T("Error while reading from pipe"),::GetLastError());
		}

		return bytesRead;
		
		
	#elif defined(XTK_OS_UNIX)
		ssize_t res = ::read(m_fdPipeIn,a.getRawData() + off, len);

		if(res == -1)
			throw xIOException(_T("Error while reading from pipe"),errno);
		if(res == 0 && len != 0)
			return xEOF;

		return (int) res;
		
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xPipeInputStream::read() throw(xIOException)
{
	if(isClosed())
		throw xIOException(_T("Cannot read data, pipe closed"));

	char buff;
	#ifdef XTK_OS_WINDOWS
		if ( !::ReadFile(m_hPipeIn, &buff, 1, NULL, NULL) )
		{
			if(::GetLastError() == ERROR_BROKEN_PIPE)
				return xEOF;
			else
				throw xIOException(_T("Error while reading from pipe"),::GetLastError());
		}

	#elif defined(XTK_OS_UNIX)
		ssize_t res = ::read(m_fdPipeIn,(void*) &buff, 1);

		if(res == -1)
			throw xIOException(_T("Error while reading from pipe"),errno);
		if(res == 0)
			return xEOF;

	#endif
	
	return (int)buff;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPipeInputStream::close() throw(xIOException)
{
	m_isClosed = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPipeOutputStream::close() throw(xIOException)
{
	m_isClosed = true;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPipeOutputStream::write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException)
{
	if(isClosed())
		throw xIOException(_T("Cannot write data, pipe closed"));
	if(len <= 0 || off+len > b.size() || off < 0)
		throw xIndexOutOfBoundsException();

	#ifdef XTK_OS_WINDOWS
		while ( len > 0 )
		{
			DWORD chunkWritten = 0;
			if (!::WriteFile(m_hPipeOut, b.getRawData() + off + (int)chunkWritten, len, &chunkWritten, NULL))
			{
				if(::GetLastError() == ERROR_BROKEN_PIPE)
				{
					close();
					throw xIOException(_T("Broken pipe, cannot write"));
				}
				else
					throw xIOException(_T("Error while writing on pipe"),::GetLastError());
			}

			if (!chunkWritten)
				break;
			len -= (int)chunkWritten;
		}

	#elif defined(XTK_OS_UNIX)
		while ( len > 0 )
		{
			ssize_t res = ::write(m_fdPipeOut, b.getRawData() + off, len);
			if(res < 0)
				throw xIOException(_T("Cannot write on pipe!"),errno);

			len -= res;
		}

	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPipeOutputStream::write(int b) throw(xIOException)
{
	if(isClosed())
		throw xIOException(_T("Cannot write data, pipe closed"));

	char c = (char)b;
	#ifdef XTK_OS_WINDOWS
		if (!::WriteFile(m_hPipeOut, &c, 1, NULL, NULL))
		{
			if(::GetLastError() == ERROR_BROKEN_PIPE)
			{
				close();
				throw xIOException(_T("Broken pipe, cannot write"));
			}
			else
				throw xIOException(_T("Error while writing on pipe"),::GetLastError());
		}

	#elif defined(XTK_OS_UNIX)
		ssize_t res = ::write(m_fdPipeOut,(const void*) &c, 1);
		if(res < 0)
			throw xIOException(_T("Cannot write on pipe!"),errno);

	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xProcess* xProcess::execute(xString filename,xArray<xString*> arguments,bool redirect)
	throw(xSystemException)
{
	#ifdef XTK_OS_WINDOWS
		STARTUPINFO si;
		ZeroMemory(&si,sizeof(si));
		si.cb = sizeof(si);
		
		HANDLE hChildStdoutRd,hChildStdoutWr,hChildStdinRd,hChildStdinWrDup,
			hChildStdinWr,hChildStderrRd,hChildStderrWr = INVALID_HANDLE_VALUE;
			
		if(redirect)
		{
			// default security attributes
			SECURITY_ATTRIBUTES security;
			security.nLength              = sizeof(security);
			security.lpSecurityDescriptor = NULL;
			security.bInheritHandle       = TRUE;
			
			// Create a pipe for the child process's STDOUT.  
			if(!::CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &security, 0))
				throw xSystemException(_T("Cannot create pipe for STDOUT redirection"),::GetLastError());
				
			// Create a pipe for the child process's STDIN.  
			if(!::CreatePipe(&hChildStdinRd, &hChildStdinWr, &security, 0))
				throw xSystemException(_T("Cannot create pipe for STDIN redirection"),::GetLastError());
				
			// Create a pipe for the child process's STDERR.  
			if(!::CreatePipe(&hChildStderrRd, &hChildStderrWr, &security, 0))
				throw xSystemException(_T("Cannot create pipe for STDERR redirection"),::GetLastError());


			si.dwFlags = STARTF_USESTDHANDLES;
			//hide the console window
			si.dwFlags |= STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
			
			si.hStdInput = hChildStdinRd;
			si.hStdOutput = hChildStdoutWr;
			si.hStdError = hChildStderrWr;
			
			
			// we must duplicate the handle to the write side of stdin pipe to make
			// it non inheritable.
			if ( !::DuplicateHandle
					(
					::GetCurrentProcess(),
					hChildStdinWr,
					::GetCurrentProcess(),
					&hChildStdinWrDup,
					0,                      // desired access: unused here
					FALSE,                  // not inherited
					DUPLICATE_SAME_ACCESS   // same access as for src handle
					))
			{
				throw xSystemException(_T("Cannot Duplicate handle for IO redirection"),::GetLastError());
			}
			::CloseHandle(hChildStdinWr);
		}
			
		PROCESS_INFORMATION pi;
		DWORD dwFlags = 0 /*CREATE_SUSPENDED*/;
		dwFlags |= CREATE_DEFAULT_ERROR_MODE ;

		xString fullcmd;
		fullcmd.append(filename);
		for(int i = 0;i < arguments.size();i++)
		{
			fullcmd.append(_T(" "));
			fullcmd.append(*(arguments[i]));
		}

		bool ok = ::CreateProcess
			(
			filename.c_str(),  // application name
			(LPTSTR) fullcmd.c_str(),   // full command line
			NULL,              // security attributes: defaults for both
			NULL,              //   the process and its main thread
			redirect,          // inherit handles if we use pipes
			dwFlags,           // process creation flags
			NULL,              // environment (use the same)
			NULL,              // current directory (use the same)
			&si,               // startup info
			&pi                // process info
			) != 0;
		
		//we close the pipe ends used by child
		if (redirect)
		{
			::CloseHandle(hChildStdinRd);
			::CloseHandle(hChildStdoutWr);
			::CloseHandle(hChildStderrWr);
		}

		if (!ok)
		{
			// close the other handles too
			if(redirect)
			{
				::CloseHandle(hChildStdoutRd);
				::CloseHandle(hChildStderrRd);
			}

			throw xSystemException(_T("Cannot start process"),::GetLastError());
		}
		
		::WaitForInputIdle(pi.hProcess,INFINITE);
		
		
		if(redirect)
			return new xProcess(pi.hProcess,pi.dwProcessId,hChildStdinWrDup,hChildStdoutRd,hChildStderrRd);
			
		return new xProcess(pi.hProcess,pi.dwProcessId,INVALID_HANDLE_VALUE,INVALID_HANDLE_VALUE,INVALID_HANDLE_VALUE);
		
		
		
	#elif defined(XTK_OS_UNIX)
	
		int fdChildStdoutRd,fdChildStdoutWr,fdChildStdinRd,
			fdChildStdinWr,fdChildStderrRd,fdChildStderrWr = -1;
			
		if(redirect)
		{	
			int pipedesc[2];
			
			//creating pipes
			if(::pipe(pipedesc) != 0)
				throw xSystemException(_T("Cannot create pipe for redirection"),errno);
			
			fdChildStdoutRd = pipedesc[0];
			fdChildStdoutWr = pipedesc[1];
			
			if(::pipe(pipedesc) != 0)
				throw xSystemException(_T("Cannot create pipe for redirection"),errno);

			fdChildStdinRd = pipedesc[0];
			fdChildStdinWr = pipedesc[1];
			
			if(::pipe(pipedesc) != 0)
				throw xSystemException(_T("Cannot create pipe for redirection"),errno);

			fdChildStderrRd = pipedesc[0];
			fdChildStderrWr = pipedesc[1];
			
		}
		
		//now fork and create process
		pid_t forkpid = ::vfork();
		if(forkpid == 0) //child process
		{
			if(redirect)
			{
				//close not needed descriptors
				::close(fdChildStdinWr);
				::close(fdChildStdoutRd);
				::close(fdChildStderrRd);
				
				//substitute default stdio descriptors
				::close(0);
				::dup(fdChildStdinRd);
				::close(fdChildStdinRd);
				
				::close(1);
				::dup(fdChildStdoutWr);
				::close(fdChildStdoutWr);
				
				::close(2);
				::dup(fdChildStderrWr);
				::close(fdChildStderrWr);
			}

			int i = 0;
			//should be enough for every purpose
			char* argv[1024];
			for(;i < arguments.size() && i < 1024;i++)
				argv[i] = (char*) arguments[i]->mb_str();
				
			argv[i] = NULL;
			
			//now start new process,the next function should not return if successful
			int ret = ::execvp(filename.mb_str(), argv);
			if(ret == -1)
				throw xSystemException(_T("Cannot start process"),errno);
		}
		else //parent process
		{
			if(redirect)
			{
				//close not needed descriptors
				::close(fdChildStdinRd);
				::close(fdChildStdoutWr);
				::close(fdChildStderrWr);
				
				return new xProcess(forkpid,fdChildStdoutRd,fdChildStderrRd,fdChildStdinWr);
			}
			
			return new xProcess(forkpid,-1,-1,-1);
		}
	
	#endif

	return NULL;
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xProcessWaitThread : public xThread
{
private:
	xProcess* m_process;
	bool	  m_mustStop;
public:
	
	xProcessWaitThread(xProcess* process)
	: xThread(xString::getFormat(_T("Thread waiting process: %ud"),process->m_pid),false)
	{
		m_process = process;
		m_mustStop = false;
	}

	virtual void run()
	{
		#ifdef XTK_OS_WINDOWS
			DWORD res;
			do
			{
				res = ::WaitForSingleObject(m_process->m_hProcess,200) != WAIT_OBJECT_0;
				if(res == WAIT_ABANDONED)
					break;
			}while(res != WAIT_OBJECT_0 && !m_mustStop);
			
			m_process->m_isRunning = false;
			::GetExitCodeProcess(m_process->m_hProcess,(LPDWORD)&(m_process->m_exitStatus));
			
		#elif defined(XTK_OS_UNIX)
			int pid, status = xInteger::MAX;
			while(!m_mustStop)
			{
				pid = ::waitpid(m_process->m_pid, &status, WNOHANG);
				if (pid == 0) //no completed
				{
					xThread::sleep(200);
					continue;
				}
				if(pid < 0) //error
					break;
				break;
			}
			
			m_process->m_exitStatus = status;
			
		#endif
		
		if(m_process->m_detatched)
			delete m_process;
	}


	virtual void terminate(){m_mustStop = true;}
	virtual ~xProcessWaitThread(){terminate();}
};



//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#ifdef XTK_OS_WINDOWS
	xProcess::xProcess(HANDLE hProcess,DWORD pid,HANDLE hWritePipeStdIn,
		HANDLE hReadPipeStdOut,HANDLE hReadPipeStdErr)
	{
		m_processWaitThread = new xProcessWaitThread(this);
		m_processWaitThread->start();
		
		m_hProcess = hProcess;
		m_pid = pid;
		m_hReadPipeStdOut = hReadPipeStdOut;
		m_hReadPipeStdErr = hReadPipeStdErr;
		m_hWritePipeStdIn = hWritePipeStdIn;
		m_isRunning = true;
		m_exitStatus = 0;
		m_detatched = false;
	}

#elif defined(XTK_OS_UNIX)
	xProcess::xProcess(int pid,int fdReadPipeStdOut,int fdReadPipeStdErr,int fdWritePipeStdIn)
	{
		m_processWaitThread = new xProcessWaitThread(this);
		m_processWaitThread->start();
		
		m_pid = pid;
		m_fdReadPipeStdOut = fdReadPipeStdOut;
		m_fdReadPipeStdErr = fdReadPipeStdErr;
		m_fdWritePipeStdIn = fdWritePipeStdIn;
		m_isRunning = true;
		m_exitStatus = 0;
		m_detatched = false;
	}
	
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xProcess::~xProcess()
{
	delete m_processWaitThread;
	kill();
	
	#ifdef XTK_OS_WINDOWS
		::CloseHandle(m_hProcess);
		if(m_hWritePipeStdIn != INVALID_HANDLE_VALUE)
			::CloseHandle(m_hWritePipeStdIn);
		if(m_hReadPipeStdOut != INVALID_HANDLE_VALUE)
			::CloseHandle(m_hReadPipeStdOut);
		if(m_hReadPipeStdErr != INVALID_HANDLE_VALUE)
			::CloseHandle(m_hReadPipeStdErr);
	#elif defined(XTK_OS_UNIX)
		if(m_fdWritePipeStdIn != -1)
			::close(m_fdWritePipeStdIn);
		if(m_fdReadPipeStdOut != -1)
			::close(m_fdReadPipeStdOut);
		if(m_fdReadPipeStdErr != -1)
			::close(m_fdReadPipeStdErr);
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xProcess::kill(xpid_t pid)
{
	#ifdef XTK_OS_WINDOWS
		HANDLE hProcess = ::OpenProcess
						(
						PROCESS_TERMINATE,		// access flag 
						FALSE,					// handle inheritance flag 
						(DWORD)pid				// process identifier 
						);
		return (::TerminateProcess(hProcess,-1) != 0);
		
	#elif defined(XTK_OS_UNIX)
		return (::kill(pid,SIGKILL) == 0);
		
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xProcess::kill()
{
	return kill(m_pid);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xProcess::wait()
{
	#ifdef XTK_OS_WINDOWS
		::WaitForSingleObject(m_hProcess,INFINITE);

	#elif defined(XTK_OS_UNIX)
		int status;
		::waitpid(m_pid, &status, 0);

	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
long xProcess::getExitStatus() throw(xIllegalStateException)
{
	if(isRunning())
		throw xIllegalStateException(_T("Cannot retrieve exit status,process yet running"));
		
	return m_exitStatus;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xInputStream* xProcess::getStdOut()
{
	#ifdef XTK_OS_WINDOWS
		if(m_hReadPipeStdOut == INVALID_HANDLE_VALUE)
			return NULL;
		
		return new xPipeInputStream(m_hReadPipeStdOut);
		
	#elif defined(XTK_OS_UNIX)
		if(m_fdReadPipeStdOut == -1)
			return NULL;

		return new xPipeInputStream(m_fdReadPipeStdOut);

	#endif
	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xInputStream* xProcess::getStdErr()
{
	#ifdef XTK_OS_WINDOWS
		if(m_hReadPipeStdErr == INVALID_HANDLE_VALUE)
			return NULL;

		return new xPipeInputStream(m_hReadPipeStdErr);

	#elif defined(XTK_OS_UNIX)
		if(m_fdReadPipeStdErr == -1)
			return NULL;

		return new xPipeInputStream(m_fdReadPipeStdErr);

	#endif

}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xOutputStream* xProcess::getStdIn()
{
	#ifdef XTK_OS_WINDOWS
		if(m_hWritePipeStdIn == INVALID_HANDLE_VALUE)
			return NULL;

		return new xPipeOutputStream(m_hWritePipeStdIn);

	#elif defined(XTK_OS_UNIX)
		if(m_fdWritePipeStdIn == -1)
			return NULL;

		return new xPipeOutputStream(m_fdWritePipeStdIn);

	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xProcess::detatch()
{
	if(!isRunning())
		delete this;
	else	
		m_detatched = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################




}//namespace

