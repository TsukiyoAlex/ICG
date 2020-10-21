#ifndef MYGL_H
#define MYGL_H

#include <stdlib.h>
#include <string.h>

#include "core.h"

void InitMyGl(void); // Inicializa o frame buffer 
unsigned char* GetFBMemPtr(void); // Retorna o ponteiro do framebuffer buffer
void CloseMyGl(void); // Desaloca a memória do framebuffer

// Declaração da função que chamará as funções implementadas pelo aluno
void MyGlDraw(void);

//
// >>> Caro aluno: declare aqui as funções que você implementar <<<
//

//Definição da estrutura de ponto
struct Point
{
	int x, y;
};

//Definição da estrutura de cor
struct Color
{
	float r, g, b, a;
};

//Declaração das funções a serem implementadas
void PutPixel(struct Point p, struct Color c);
void DrawLine(struct Point p0, struct Color c0, struct Point p1, struct Color c1);
void DrawTriangle(struct Point p0, struct Color c0, struct Point p1, struct Color c1, struct Point p2, struct Color c2);

#endif  // MYGL_H
