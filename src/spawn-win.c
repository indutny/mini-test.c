#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#include <strsafe.h>

#include "include/mini/common.h"

#define BUFSIZE 4096 

static char executable_path[BUFSIZE];

static int popen_complete(const char* cmd) {
  STARTUPINFO StartupInfo;
  ZeroMemory(&StartupInfo, sizeof(StartupInfo));
  StartupInfo.cb = sizeof(StartupInfo);

  PROCESS_INFORMATION ProcessInformation;
  ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));

  /*
    BOOL WINAPI CreateProcess(
      _In_opt_    LPCTSTR               lpApplicationName,
      _Inout_opt_ LPTSTR                lpCommandLine,
      _In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
      _In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
      _In_        BOOL                  bInheritHandles,
      _In_        DWORD                 dwCreationFlags,
      _In_opt_    LPVOID                lpEnvironment,
      _In_opt_    LPCTSTR               lpCurrentDirectory,
      _In_        LPSTARTUPINFO         lpStartupInfo,
      Out_       LPPROCESS_INFORMATION lpProcessInformation
    );
  */
  BOOL ret = CreateProcess(
    NULL,                     // No module name (use command line)
    (char*)cmd,               // Command line
    NULL,                     // Process handle not inheritable
    NULL,                     // Thread handle not inheritable
    FALSE,                    // Set handle inheritance to FALSE
    0,
    NULL,                     // Use parent's environment block
    NULL,                     // Use parent's starting directory 
    &StartupInfo,             // Pointer to STARTUPINFO structure
    &ProcessInformation       // Pointer to PROCESS_INFORMATION structure
  );
  CHECK_EQ(ret, TRUE, "CreateProcess failed");

  // Wait until child process exits.
  DWORD wait_ret = WaitForSingleObject(ProcessInformation.hProcess, INFINITE);
  CHECK_EQ(wait_ret, WAIT_OBJECT_0, "Process ended strange");
  
  // Get exit code (-1 is good)
  DWORD lpExitCode = 0;
  BOOL exit_ret = GetExitCodeProcess(ProcessInformation.hProcess, &lpExitCode);

  // Close process and thread handles.
  CloseHandle(ProcessInformation.hProcess);
  CloseHandle(ProcessInformation.hThread);

  return lpExitCode;
}


void mini_prepare_runner(const char* main) {
  TCHAR** lppPart = { NULL };
  GetFullPathName(main, BUFSIZE, executable_path, lppPart);
  CHECK_NE(executable_path, NULL, "realpath(argv[0])");
}


int mini_run_single(const char* test) {
  char cmd[BUFSIZE];
  int print_len = sprintf_s(cmd, BUFSIZE, "\"%s\" %s", executable_path, test);
  CHECK_NE(print_len, -1, "sprintf_s to format cmd failed");
  int ret = popen_complete(cmd);
  return ret;
}
