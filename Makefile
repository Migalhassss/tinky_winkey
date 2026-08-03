INC = /I incs
CFLAGS = $(INC) /c /wd5045

SVC_OBJ = build\svc.obj build\install.obj build\delete.obj build\start.obj build\stop.obj

WINKEY_OBJ = build\winkey.obj build\window.obj build\keyboard.obj build\logger.obj build\hook.obj

all: create_build_dir svc.exe winkey.exe

create_build_dir:
	@if not exist build mkdir build

svc.exe: $(SVC_OBJ)
	link $(SVC_OBJ) user32.lib advapi32.lib kernel32.lib /OUT:svc.exe

winkey.exe: $(WINKEY_OBJ)
	link $(WINKEY_OBJ) user32.lib advapi32.lib kernel32.lib /OUT:winkey.exe

build\svc.obj: srcs\svc.c
	cl $(CFLAGS) srcs\svc.c /Fo:build\svc.obj

build\install.obj: srcs\install.c
	cl $(CFLAGS) srcs\install.c /Fo:build\install.obj

build\delete.obj: srcs\delete.c
	cl $(CFLAGS) srcs\delete.c /Fo:build\delete.obj

build\start.obj: srcs\start.c
	cl $(CFLAGS) srcs\start.c /Fo:build\start.obj

build\stop.obj: srcs\stop.c
	cl $(CFLAGS) srcs\stop.c /Fo:build\stop.obj

build\winkey.obj: srcs\winkey.c
	cl $(CFLAGS) srcs\winkey.c /Fo:build\winkey.obj

build\window.obj: srcs\window.c
	cl $(CFLAGS) srcs\window.c /Fo:build\window.obj

build\keyboard.obj: srcs\keyboard.c
	cl $(CFLAGS) srcs\keyboard.c /Fo:build\keyboard.obj

build\logger.obj: srcs\logger.c
	cl $(CFLAGS) srcs\logger.c /Fo:build\logger.obj

build\hook.obj: srcs\hook.c
	cl $(CFLAGS) srcs\hook.c /Fo:build\hook.obj

clean:
    del build\*.obj
    del winkey.exe
	del svc.exe

re:
	nmake clean
	nmake all