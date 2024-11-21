#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#define ENTER 13

using namespace std;

void gotoxy(int x, int y){

    HANDLE conl;
    conl=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD post;
    post.X=x;
    post.Y=y;

    SetConsoleCursorPosition(conl,post);
    }


void OcultarCursor() {
    HANDLE conl;
    conl = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO pti;
    pti.dwSize = 50;
    pti.bVisible = FALSE;

    SetConsoleCursorInfo(conl, &pti);
}

void cambiar_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

vector<pair<int,int>>obstaculos;

void dibujar_fondo(){

     for (int i = 2; i <= 77; i++) {
        gotoxy(i, 2); printf("%c",205);
        gotoxy(i, 33);printf("%c",205);
    }
    for (int i = 3; i < 33; i++) {
        gotoxy(2, i);printf("%c",186);
        gotoxy(77, i);printf("%c",186);
    }

    gotoxy(2, 2);printf("%c",201);
    gotoxy(77, 2);printf("%c",187);
    gotoxy(2, 33);printf("%c",200);
    gotoxy(77, 33);printf("%c",188);

    for (int i = 15; i < 70; i += 10) {
        gotoxy(i, 28);printf("%c",219);
        obstaculos.push_back({i,28});
    }
    for (int i = 25; i < 25; i += 5) {
        gotoxy(i, 30); printf("%c", 219);
        obstaculos.push_back({i, 30});
    }

    for (int i = 5; i < 30; i += 10) {
        gotoxy(10, i); printf("%c", 219);
        obstaculos.push_back({10, i});
        gotoxy(72, i + 1); printf("%c", 219);
        obstaculos.push_back({72, i + 1});
    }
}

void mostrarMenu(vector<string> opciones, int seleccion) {
    gotoxy(32, 8); printf("=== MENU PRINCIPAL ===");
    for (int i = 0; i < opciones.size(); i++) {
            if (i == seleccion) {
                cambiar_color(10);
                }
    else {
            cambiar_color(15);
    }
    gotoxy(32, 10 + i); cout << opciones[i];
     }

}

void instrucciones() {
    system("cls");
    gotoxy(20, 15); printf("Usa las flechas del teclado para mover el personaje");
    gotoxy(20, 17); printf("Objetivo: Evita los obstaculos y sobrevive.");
    gotoxy(20, 19); printf("Presiona cualquier tecla para volver al menu...");
    _getch();
}

int manejarMenu() {
    vector<string>opciones={"Start game","Intruciones","Exit"};
    int seleccion=0;
    cambiar_color(10);
    while (true) {
        cambiar_color(10);
        system("cls");
        mostrarMenu(opciones, seleccion);
        int tecla = _getch();

        switch(tecla) {
            case ARRIBA:
                seleccion=(seleccion-1 + opciones.size()) % opciones.size();
                break;
            case ABAJO:
                seleccion=(seleccion+1) % opciones.size();
                break;
            case ENTER:
                return seleccion;
            default:
                break;
        }
    }
}


class serpiente{
    vector<pair<int, int>> cuerpo;
    int direccion;

public:
    serpiente(int x,int y){
        cuerpo.push_back({x,y});
        direccion = DERECHA;
    }
    void pintar();
    void mover(bool &gameOver,int &vidas);
    void borrar();
    void cambiar_direccion(int nueva_direccion);
    void crecer();
    void mostrar_puntos(int puntos);

    pair<int, int>obtener_cabeza(){
        return cuerpo[0];
    }
};

void serpiente::pintar(){
    cambiar_color(10);
    gotoxy(cuerpo[0].first,cuerpo[0].second);
    printf("O");

    cambiar_color(2);
    for(int i=1;i< cuerpo.size();i++){
        gotoxy(cuerpo[i].first,cuerpo[i].second);
        printf("*");
    }
}
void serpiente::borrar(){
    auto cola= cuerpo.back();
    gotoxy(cola.first,cola.second);
    printf(" ");
}
void redibujar_obstaculos() {
    for (const auto& obstaculo : obstaculos) {
        gotoxy(obstaculo.first, obstaculo.second);
        printf("%c", 219);
    }
}

void serpiente::mover(bool &gameOver, int &vidas){
    borrar();
    if (cuerpo[0].first <= 3 || cuerpo[0].first >= 76 || cuerpo[0].second <= 3 || cuerpo[0].second >= 32) {
        gameOver = true;
        return;
    }

    for(int i=cuerpo.size()-1;i>0;--i){
        cuerpo[i]=cuerpo[i-1];
    }
    switch (direccion) {
            case ARRIBA:
                cuerpo[0].second--;
                break;
            case ABAJO:
                cuerpo[0].second++;
                break;
            case IZQUIERDA:
                cuerpo[0].first--;
                break;
            case DERECHA:
                cuerpo[0].first++;
                break;
        }
        if (cuerpo[0].first <= 2 || cuerpo[0].first >= 77 || cuerpo[0].second <= 2 || cuerpo[0].second >= 33) {
        gameOver = true;
    }
    for (int i = 1; i < cuerpo.size(); ++i) {
        if (cuerpo[0] == cuerpo[i]) {
            gameOver = true;
            return;
        }
    }
        redibujar_obstaculos();
        for (auto &obstaculo : obstaculos) {
        if (cuerpo[0] == obstaculo) {
            vidas--;

            if(vidas<=0){
                gameOver=true;
            }else {
                gotoxy(cuerpo[0].first, cuerpo[0].second);
                printf(" ");
                cuerpo[0] = cuerpo.back();

            }
            return;
            pintar();



        }

    }


        pintar();
        Sleep(1);
}
void serpiente::cambiar_direccion(int nueva_direccion) {
        if ((direccion == ARRIBA && nueva_direccion != ABAJO) ||
            (direccion == ABAJO && nueva_direccion != ARRIBA) ||
            (direccion == IZQUIERDA && nueva_direccion != DERECHA) ||
            (direccion == DERECHA && nueva_direccion != IZQUIERDA)) {
            direccion = nueva_direccion;
        }
    }

    void serpiente::crecer(){
        cuerpo.push_back(cuerpo.back());
    }
void serpiente::mostrar_puntos(int puntos){
     gotoxy(2, 1);
    printf("Puntos: %d", puntos);
}

void mostrar_vidas(int vidas){
    gotoxy(20, 1);
    printf("Vidas: %d", vidas);
}
int main(){
    int puntos=0;
    int vidas;
    OcultarCursor();
    cambiar_color(10);
while(true) {
    int opcion=manejarMenu();
    if(opcion==2){
        exit(0);
    }else if(opcion==1){
    instrucciones ();
    }
    else if(opcion==0){
    system("cls");
    vidas=3;
    puntos=0;
    cambiar_color(10);
    dibujar_fondo();
    srand(time(0));
    serpiente serpiente(10,10);
    serpiente.pintar();

    bool gameOver = false;
    int comidaX = (rand() % (71-4)) + 4;
    int comidaY = (rand() % (27-4)) + 4;

    cambiar_color(10);
    gotoxy(comidaX, comidaY); printf("@");

    while (!gameOver) {
        if (_kbhit()) {
            char tecla = _getch();
            switch (tecla) {
                case ARRIBA: case ABAJO: case IZQUIERDA: case DERECHA:
                    serpiente.cambiar_direccion(tecla);
                    break;
            }
        }

        serpiente.mover(gameOver, vidas);

       if (serpiente.obtener_cabeza().first == comidaX && serpiente.obtener_cabeza().second == comidaY) {
            serpiente.crecer();
            puntos += 10;
            comidaX = (rand() % (71-4)) + 4;
            comidaY = (rand() % (27-4)) + 4;
            cambiar_color(10);
            gotoxy(comidaX, comidaY); printf("@");

        }


        serpiente.mostrar_puntos(puntos);
        mostrar_vidas(vidas);

        if (gameOver) {
                gotoxy(30, 15); printf("__¡Game Over!__");
                gotoxy(15, 17); printf("Presiona cualquier tecla para volver al menu...");
                _getch();
                break;
        }

        Sleep(120);

    }
    }
  }
    return 0;
}
