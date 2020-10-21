#include "mygl.h"

unsigned char* fb_ptr = NULL;

void InitMyGl(void) {
    fb_ptr = (unsigned char *)malloc(IMAGE_WIDTH * IMAGE_HEIGHT * 4 * sizeof(char));	

    for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; ++i) {
        fb_ptr[i * 4] = 0;
        fb_ptr[i * 4 + 1] = 0;
        fb_ptr[i * 4 + 2] = 0;
        fb_ptr[i * 4 + 3] = 255;
    }    
}

unsigned char* GetFBMemPtr(void) {
	return fb_ptr;
}

void CloseMyGl(void) {
    if (!fb_ptr)
        free(fb_ptr);
}

//
// >>> Caro aluno: defina aqui as funções que você implementar <<<
//

void PutPixel(struct Point p, struct Color c)
{
	//Calcula o offset do ponteiro no color buffer	
	int offset = p.x * 4 + p.y * 4 * IMAGE_WIDTH;
	//Adiciona a informação de cor do pixel no color buffer
	fb_ptr[offset] = c.r;
	fb_ptr[offset + 1] = c.g;
	fb_ptr[offset + 2] = c.b;
	fb_ptr[offset + 3] = c.a;
}

void DrawLine(struct Point p0, struct Color c0, struct Point p1, struct Color c1)
{
	int x = p0.x;
	int y = p0.y;
	int xf = p1.x;
	int yf = p1.y;
	
	struct Color c = c0;
	
	int aux = 1;
	int auy = 1;
	
	int dx = xf - x;
	if (dx < 0) {dx *= -1; aux = -1;}
	int dy = yf - y;
	if (dy < 0) {dy *= -1; auy = -1;}
	
	PutPixel(p0, c0);
	
	if (dy <= dx){
		int D = 2 * dy - dx;
		int inc_E = 2 * dy;
		int inc_NE = 2 * (dy - dx);
		
		float dr = (c1.r - c0.r)/dx;
		float dg = (c1.g - c0.g)/dx;
		float db = (c1.b - c0.b)/dx;
		
		while(x * aux < xf){
			if (D <= 0){
				D += inc_E;
				x = x + 1 * aux;
			} else{
				D += inc_NE;
				x = x + 1 * aux;
				y = y + 1 * auy;
			}
			
			c.r += dr;
			c.g += dg;
			c.b += db;
			
			struct Point p = {x, y};

			PutPixel(p, c);
	 	}
	} else{
		int D = 2 * dx - dy;
		int inc_E = 2 * dx;
		int inc_NE = 2 * (dx - dy);
		
		float dr = (c1.r - c0.r)/dy;
		float dg = (c1.g - c0.g)/dy;
		float db = (c1.b - c0.b)/dy;
		
		while(y * auy < yf){
			if (D <= 0){
				D += inc_E;
				y = y + 1 * auy;
			} else{
				D += inc_NE;
				x = x + 1 * aux;
				y = y + 1 * auy;
			}
			
			c.r += dr;
			c.g += dg;
			c.b += db;
			
			struct Point p = {x, y};
			
			PutPixel(p, c);
		}
	}
}

void DrawTriangle(struct Point p0, struct Color c0, struct Point p1, struct Color c1, struct Point p2, struct Color c2)
{
	DrawLine(p0, c0, p1, c1);
	DrawLine(p1, c1, p2, c2);
	DrawLine(p2, c2, p0, c0);
}

// Definição da função que chamará as funções implementadas pelo aluno
void MyGlDraw(void) {
    //
    // >>> Caro aluno: chame aqui as funções que você implementou <<<
    //
    
    //Inicialização de uma paleta de cores
    struct Color red = {255, 0, 0, 255};
    struct Color orange = {252, 132, 3, 255};
    struct Color yellow = {255, 255, 0, 255};
    struct Color green = {0, 255, 0, 255};
    struct Color cyan = {0, 255, 255, 255};
    struct Color blue = {0, 0, 255, 255};
    struct Color purple = {166, 0, 255, 255};
    struct Color magenta = {255, 0, 255, 255};
    struct Color white = {255, 255, 255, 255};
    
    //Teste 1: Pinta um "arco-íris" horizontal na tela
    /*{
    	//Define a altura de cada cor
		int h1 = IMAGE_HEIGHT/10;
    	int h2 = 2*h1;
    	int h3 = 3*h1;
    	int h4 = 4*h1;
    	int h5 = 5*h1;
    	int h6 = 6*h1;
    	int h7 = 7*h1;
    	int h8 = 8*h1;
    	int h9 = 9*h1;
    	//Linha vermelha
    	for (int i = 0; i < h1; i++){
    		for (int j = 0; j < IMAGE_WIDTH; j++){
    			struct Point p = {j, i};
    			PutPixel(p, red);
			}
		}
		//Linha laranja
    	for (int i = h1; i < h2; i++){
    		for (int j = 0; j < IMAGE_WIDTH; j++){
    			struct Point p = {j, i};
    			PutPixel(p, orange);
			}
		}
		//Linha amarela
		for (int i = h2; i < h3; i++){
    		for (int j = 0; j < IMAGE_WIDTH; j++){
    			struct Point p = {j, i};
    			PutPixel(p, yellow);
			}
		}
		//Linha verde
		for (int i = h3; i < h4; i++){
	    	for (int j = 0; j < IMAGE_WIDTH; j++){
	    		struct Point p = {j, i};
	    		PutPixel(p, green);
			}
		}
		//Linha ciano
		for (int i = h4; i < h5; i++){
	    	for (int j = 0; j < IMAGE_WIDTH; j++){
	    		struct Point p = {j, i};
	    		PutPixel(p, cyan);
			}
		}
		//Linha azul
		for (int i = h5; i < h6; i++){
	    	for (int j = 0; j < IMAGE_WIDTH; j++){
	    		struct Point p = {j, i};
	    		PutPixel(p, blue);
			}
		}
		//Linha roxa
    	for (int i = h6; i < h7; i++){
    		for (int j = 0; j < IMAGE_WIDTH; j++){
    			struct Point p = {j, i};
    			PutPixel(p, purple);
			}
		}
		//Linha magenta
		for (int i = h7; i < h8; i++){
	    	for (int j = 0; j < IMAGE_WIDTH; j++){
	    		struct Point p = {j, i};
	    		PutPixel(p, magenta);
			}
		}
		//Linha branca
		for (int i = h8; i < h9; i++){
	    	for (int j = 0; j < IMAGE_WIDTH; j++){
	    		struct Point p = {j, i};
	    		PutPixel(p, white);
			}
		}
	}*/
	
	//Teste 2: Pinta um quadrado azul de tamanho 100x100 no centro da tela
	/*{
		//Calcula o pixel central da tela
		int xcenter = IMAGE_WIDTH/2;
		int ycenter = IMAGE_HEIGHT/2;
		//Calcula os vértices do quadrado
		int Ax = xcenter + 50;
		int Ay = ycenter + 50;
		int Bx = xcenter - 50;
		int By = ycenter - 50;
		
		//Para evitar redundância os seguintes vértices foram omitidos pois suas coordenadas já foram calculadas nos vértices A e B
		//int Cx = Ax;
		//int Cy = By;
		//int Dx = Bx;
		//int Dy = Ay;
		
		//Pinta o primeiro quadrante (vértice A)
		for (int i = xcenter; i < Ax; i++){
			for (int j = ycenter; j < Ay; j++){
				struct Point p = {j, i};
				PutPixel(p, blue);
			}
		}
		//Pinta o segundo quadrante (vértice D)
		for (int i = xcenter; i > Bx; i--){
			for (int j = ycenter; j < Ay; j++){
				struct Point p = {j, i};
				PutPixel(p, blue);
			}
		}
		//Pinta o terceiro quadrante (vértice B)
		for (int i = xcenter; i > Bx; i--){
			for (int j = ycenter; j > By; j--){
				struct Point p = {j, i};
				PutPixel(p, blue);
			}
		}
		//Quarto quadrante (vértice C)
		for (int i = xcenter; i < Ax; i++){
			for (int j = ycenter; j > By; j--){
				struct Point p = {j, i};
				PutPixel(p, blue);
			}
		}
	}*/
	
	//Teste 3: Desenha uma linha branca do canto inferior esquerdo da tela ao canto superior direito da tela
	/*{
		struct Point p0 = {0, 0};
		struct Point p1 = {IMAGE_WIDTH - 1, IMAGE_HEIGHT - 1};
		DrawLine(p0, white, p1, white);
	}*/
	
	//Teste 4: Desenha 8 linhas coloridas que vão do centro da tela aos seus cantos, uma em cada octante
	/*{
		//Calcula o pixel central da tela
		int xcenter = IMAGE_WIDTH/2;
		int ycenter = IMAGE_HEIGHT/2;
		//Define alguns valores para x e y interessantes
		int x0 = IMAGE_WIDTH - 1;
		int x1 = 3 * xcenter / 2;
		int x2 = xcenter / 2;
		int y0 = IMAGE_HEIGHT - 1;
		int y1 = 3 * ycenter / 2;
		int y2 = ycenter / 2;
		//Define os pontos central e de cada octante
		struct Point center = {xcenter, ycenter};
		struct Point oct1 = {x0, y1};
		struct Point oct2 = {x1, y0};
		struct Point oct3 = {x2, y0};
		struct Point oct4 = {0, y1};
		struct Point oct5 = {0, y2};
		struct Point oct6 = {x2, 0};
		struct Point oct7 = {x1, 0};
		struct Point oct8 = {x0, y2};
		//Desenha todas as linhas
		DrawLine(center, white, oct1, red);
		DrawLine(center, white, oct2, orange);
		DrawLine(center, white, oct3, yellow);
		DrawLine(center, white, oct4, green);
		DrawLine(center, white, oct5, cyan);
		DrawLine(center, white, oct6, blue);
		DrawLine(center, white, oct7, purple);
		DrawLine(center, white, oct8, magenta);
	}*/
	
	//Teste 5: Desenha 1 triângulo na tela
	{
		struct Point p0 = {0, 0};
		struct Point p1 = {IMAGE_WIDTH/2, IMAGE_HEIGHT - 1};
		struct Point p2 = {IMAGE_WIDTH - 1, 0};
	
		DrawTriangle(p0, red, p1, green, p2, blue);
	}
}
