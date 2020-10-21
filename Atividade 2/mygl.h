#ifndef MYGL_H
#define MYGL_H

#include <stdlib.h>
#include <string.h>

#include "core.h"

void InitMyGl(void); // Inicializa o frame buffer 
unsigned char* GetFBMemPtr(void); // Retorna o ponteiro do framebuffer buffer
void CloseMyGl(void); // Desaloca a mem�ria do framebuffer

// Declara��o da fun��o que chamar� as fun��es implementadas pelo aluno
void MyGlDraw(void);

//
// >>> Caro aluno: declare aqui as fun��es que voc� implementar <<<
//

//Defini��o da estrutura de ponto
struct Point
{
	int x, y;
};

//Defini��o da estrutura de cor
struct Color
{
	float r, g, b, a;
};

//Declara��o das fun��es a serem implementadas
void PutPixel(struct Point p, struct Color c);
void DrawLine(struct Point p0, struct Color c0, struct Point p1, struct Color c1);
void DrawTriangle(struct Point p0, struct Color c0, struct Point p1, struct Color c1, struct Point p2, struct Color c2);

#endif  // MYGL_H
