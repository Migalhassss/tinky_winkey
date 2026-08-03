#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "service.h"
#include "hook.h"
#include "logger.h"

SERVICE_STATUS_HANDLEg_status_handle = NULL;
SERVICE_STATUS			g_status = {0};
HANDLE					g_service_stop_event = NULL;

void WINAPI ServiceCtrlHandler(DWORD request)
{
	if (request == SERVICE_CONTROL_STOP)
	{
		// 1. Dizemos ao Windows que estamos a parar
		g_status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(g_status_handle, &g_status);

		// 2. Mudamos o estado global para STOPPED
		// Isso faz o ciclo while na ServiceMain quebrar suavemente!
		g_status.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(g_status_handle, &g_status);
	}
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	g_status_handle = RegisterServiceCtrlHandler("tinky", ServiceCtrlHandler);
	if (!g_status_handle)
		return;
	report_service_status(SERVICE_START_PENDING);
	g_

	g_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_status.dwCurrentState = SERVICE_START_PENDING;
	
	g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	SetServiceStatus(g_status_handle, &g_status);

	// Agora sim, mudamos para RUNNING
	g_status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(g_status_handle, &g_status);

	while (g_status.dwCurrentState == SERVICE_RUNNING)
	{
		Sleep(100);
	}
	
	hook_uninstall();
	logger_close();

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
