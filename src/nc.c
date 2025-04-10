#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include "fs.h"
#include "tcat.h"

char *WARFILE;
char *CATALINA_HOME;

typedef struct Button_t Button;
struct Button_t
{
    int x, y;
    char *text;
    void (*eventlistener)(Button *self, MEVENT event);
    void (*callback)(Button *self, MEVENT event);
    void (*draw)(Button *self);
};

typedef struct Server_t Server;
struct Server_t
{
    char *state;
    char *host;
    char *callbackinfo;
    char *warfile;
    int c_state, c_host, c_call;
};

void btn_draw(Button *self)
{
    attron(COLOR_PAIR(1));
    mvprintw(self->y, self->x, self->text);
    attroff(COLOR_PAIR(1));
}

void btn_ev_listener(Button *self, MEVENT event)
{
    if (event.y >= self->y && event.y <= self->y + 2 && event.x >= self->x && event.x <= self->x + strlen(self->text))
    {
        self->callback(self, event);
    }
}

void draw_server(Server *server)
{
    mvprintw(1, 0, "TOMCAT:");
    attron(COLOR_PAIR(server->c_state));
    mvprintw(2, 2, server->state);
    attroff(COLOR_PAIR(server->c_state));

    mvprintw(3, 0, "HOST:");
    attron(COLOR_PAIR(server->c_host));
    mvprintw(4, 2, server->host);
    attroff(COLOR_PAIR(server->c_host));

    mvprintw(5, 0, "CALLBACK:");
    attron(COLOR_PAIR(server->c_call));
    mvprintw(6, 2, server->callbackinfo);
    attroff(COLOR_PAIR(server->c_call));
}

Server *server;

void manage_result(Server *server, int result, char *msgi, char *msge)
{
    if (result == 0)
    {
        server->callbackinfo = msgi;
        server->c_call = 2;
    }
    else
    {
        server->callbackinfo = msge;
        server->c_call = 4;
    }
}

void init();

void set_callback(Button *self, MEVENT event)
{
    endwin();
    printf("Presioná Enter para volver...\n");
    int result = cpywar(WARFILE, CATALINA_HOME);
    if (result != 0)
        perror("copy file");
    getchar();
    manage_result(server, result, "Archivo WARFILE copiado exitosamente", "No se pudo copiar el archivo WARFILE");
    init();
}

void run_callback(Button *self, MEVENT event)
{
    endwin();
    printf("Presioná Enter para volver...\n");
    int result = runsh(startup, WARFILE, CATALINA_HOME);
    if (result != 0)
        perror("tomcat");
    getchar();
    manage_result(server, result, "Servidor encendido", "Error al intentar encender tomcat");
    if (result == 0)
    {
        server->state = "[ENCENDIDO]";
        server->c_state = 3;
    }
    init();
}
void stop_callback(Button *self, MEVENT event)
{
    endwin();
    printf("Presioná Enter para volver...\n");
    int result = runsh(shutdown, WARFILE, CATALINA_HOME);
    if (result != 0)
        perror("copy file");
    getchar();
    manage_result(server, result, "Servidor tomcat detenido", "Error al intentar apagar tomcat");
    if (result == 0)
    {
        server->state = "[APAGADO]";
        server->c_state = 4;
    }
    init();
}
void rm_callback(Button *self, MEVENT event)
{
    endwin();
    printf("Presioná Enter para volver...\n");
    int result = rmwar(WARFILE, CATALINA_HOME);
    if (result != 0)
        perror("copy file");
    getchar();
    manage_result(server, result, "WARFILE y folder eliminados", "No se pudo eliminar el WARFILE o la carpeta");
    init();
}

Button btns[4] = {
    {10, 20, "[ SET ] ", &btn_ev_listener, &set_callback, &btn_draw},
    {20, 20, "[ RUN ]", &btn_ev_listener, &run_callback, &btn_draw},
    {30, 20, "[ STOP ]", &btn_ev_listener, &stop_callback, &btn_draw},
    {40, 20, "[ REMOVE FILES ]", &btn_ev_listener, &rm_callback, &btn_draw},
};

void draw()
{
    clear();
    for (int i = 0; i < 4; i++)
        btns[i].draw(&btns[i]);

    draw_server(server);
    refresh();
}

void init()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);

    draw();
}

void nc_loop(MEVENT event)
{

    while (1)
    {
        int ch = getch();
        if (ch == KEY_MOUSE)
        {
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED)
                {
                    for (int i = 0; i < 4; i++)
                        btns[i].eventlistener(&btns[i], event);
                }
            }
        }
        else if (ch == 'q')
        {
            break;
        }
    }

    endwin();
}

void nc_listen(char *warfile, char *catalina_home)
{

    WARFILE = warfile;
    CATALINA_HOME = catalina_home;

    const char *localhost = "http://localhost:8080/";

    server = (Server *)malloc(sizeof(Server));
    server->state = (char *)malloc(sizeof(char) * 16);
    server->state = "[APAGADO]";
    server->host = (char *)malloc(sizeof(char) * (strlen(localhost) + strlen(warfile)));
    strcat(server->host, localhost);
    strcat(server->host, warfile);
    server->callbackinfo = (char *)malloc(sizeof(char) * 128);
    server->warfile = (char *)malloc(sizeof(char) * strlen(warfile));
    strcat(server->warfile, warfile);
    server->c_state = 4;
    server->c_host = 2;
    server->c_call = 2;

    MEVENT event;
    init();
    nc_loop(event);

    free(server);
}