/*****************************************************************************
 * Este programa � baseado no exemplo "Hello-Triangle" dispon�vel 
 * em 
 *     https://learnopengl.com/Getting-started/Hello-Triangle
 * 
 *     A principal diferen�a entre este programa e o exemplo acima � que neste
 * os vertex e fragment shaders s�o carregados a partir de arquivos externos, 
 * ao inves de estarem hard coded no c�digo fonte. 
 *     Isso da mais flexibilidade para se fazerem experimentos com os shaders, 
 * pois nao e necessario se recompilar o programa a cada vez que os shaders forem
 * alterados.
 ****************************************************************************/
#include "main.h"

#include <cmath>

#define IMAGE_WIDTH 512 // Largura da janela OpenGL em pixels.
#define IMAGE_HEIGHT 512 // Altura da janela OpenGL em pixels.

// Array contendo as coordenadas X,Y e Z de tres vertices (um trianglulo).
float vertices[] = {-0.25f, -0.5f, -0.1f, 0.75f, 0.0f, 0.0f, // red triangle (closer)
                     0.25f,  0.5f, -0.1f, 0.75f, 0.0f, 0.0f,
                     0.75f, -0.5f, -0.1f, 0.75f, 0.0f, 0.0f,
                    -0.75f, -0.5f, -0.4f, 0.0f, 0.0f, 0.75f, // blue triangle (farther)
                    -0.25f,  0.5f, -0.4f, 0.0f, 0.0f, 0.75f,
                     0.25f, -0.5f, -0.4f, 0.0f, 0.0f, 0.75f}; 

char* frag_shader_source = NULL;
char* vertex_shader_source = NULL;
unsigned int shader_program;
unsigned int vbo; // Vertex buffer object ID
unsigned int vao; // Vertex array object ID

//********************************************************************************************************************
// A fun��o LoadShader() � baseada em https://stackoverflow.com/a/174552/6107321
void LoadShader(char* file_name, char** shader_source) {
    long length;
    FILE* f = fopen(file_name, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        (*shader_source) = (char*)malloc(length + 1);
        if ((*shader_source)) {
            fread((*shader_source), 1, length, f);
            (*shader_source)[length] = '\0';
        }

        fclose(f);
    } else {
        printf("Could not load the shader file %s\nExiting...", file_name);
        exit(EXIT_FAILURE);
    }
}
//********************************************************************************************************************
// Cross Product Function, calculates AxB, and stores the result in C
void CrossP(float *A, float *B, float *C) {
	C[0] = (A[1]*B[2] - A[2]*B[1]);
	C[1] = (A[2]*B[0] - A[0]*B[2]);
	C[2] = (A[0]*B[1] - A[1]*B[0]);
}
//********************************************************************************************************************
// Vector Norm Function, calculates V norm
float Norm(float *V) {
	float sum = 0.0f;
	for(int w = 0; w < 3; w++){sum = sum + V[w] * V[w];}
	return sqrt(sum);
}
//********************************************************************************************************************
void Display(void) {
    // Limpa a tela e o depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Define a posi��o da Viewport dentro da janela OpenGL
    glViewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);

    // Seleciona o Shader Program a ser utilizado.
    glUseProgram(shader_program);

    // Matriz Model ///////////////////////////////////////////////////////////
    // You will have to change the contents of this matrix for the exercises
    float model_array[16] = {1.0f, 0.0f, 0.0f, 0.0f, 
                             0.0f, 1.0f, 0.0f, 0.0f, 
                             0.0f, 0.0f, 1.0f, 0.0f, 
                             0.0f, 0.0f, 0.0f, 1.0f};
    glm::mat4 model_mat = glm::make_mat4(model_array);
    
    // Scale //////////////////////////////////////////////////////////////////
    
    // Parameters #############################################################
    float sx = 1.0f;
    float sy = 1.0f;
    float sz = 1.0f;
    //#########################################################################
    
    float scale_array[16] = {sx,   0.0f, 0.0f, 0.0f,
    						 0.0f, sy,   0.0f, 0.0f,
    						 0.0f, 0.0f, sz,   0.0f,
    						 0.0f, 0.0f, 0.0f, 1.0f};
    						 
    glm::mat4 scale_mat = glm::make_mat4(scale_array);
    ///////////////////////////////////////////////////////////////////////////
    
    // Translation ///////////////////////////////////////////////////////////
    
    // Parameters #############################################################
    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;
    //#########################################################################
    
    float translation_array[16] = {1.0f, 0.0f, 0.0f, 0.0f,
    							   0.0f, 1.0f, 0.0f, 0.0f,
    							   0.0f, 0.0f, 1.0f, 0.0f,
    							   dx,   dy,   dz,   1.0f};
    							   
    glm::mat4 translation_mat = glm::make_mat4(translation_array);
    //////////////////////////////////////////////////////////////////////////
    
    // Composing transformations ##############################################
    model_mat = scale_mat * translation_mat;
    //#########################################################################

    // Matriz View ////////////////////////////////////////////////////////////
    // You will have to change the contents of this matrix for the exercises
    float view_array[16] = {1.0f, 0.0f, 0.0f, 0.0f, 
                            0.0f, 1.0f, 0.0f, 0.0f, 
                            0.0f, 0.0f, 1.0f, 0.0f, 
                            0.0f, 0.0f, 0.0f, 1.0f};
                            
    glm::mat4 view_mat = glm::make_mat4(view_array);
	
	// Camera parameters ######################################################
    float cam_position[3] = {0.0f, 0.0f, 0.0f};
    float cam_point[3] = {0.0f, 0.0f, -1.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    //#########################################################################
    
    float cam_direction[3];
    for(int w = 0; w < 3; w++){cam_direction[w] = cam_point[w] - cam_position[w];}
    
	float z_cam[3];
    float z = Norm(cam_direction);
    for(int w = 0; w < 3; w++){z_cam[w] = -1 * cam_direction[w]/z;}
    
    float x_cam[3];
    CrossP(cam_up, z_cam, x_cam);
    float x = Norm(x_cam);
    for(int w = 0; w < 3; w++){x_cam[w] = x_cam[w]/x;}
    
    float y_cam[3];
    CrossP(z_cam, x_cam, y_cam);
    
    float BT_array[16] = {x_cam[0], y_cam[0], z_cam[0], 0.0f,
						  x_cam[1], y_cam[1], z_cam[1], 0.0f,
						  x_cam[2], y_cam[2], z_cam[2], 0.0f,
						  0.0f,     0.0f,     0.0f,     1.0f};
	
	glm::mat4 BT_mat = glm::make_mat4(BT_array);
	
	float p[3];
	for(int w = 0; w < 3; w++){p[w] = -1 * cam_position[w];}
	
	float T_array[16] = {1.0f, 0.0f, 0.0f, 0.0f,
				   		 0.0f, 1.0f, 0.0f, 0.0f,
				   		 0.0f, 0.0f, 1.0f, 0.0f,
				   		 p[0], p[1], p[2], 1.0f};
				   
	glm::mat4 T_mat = glm::make_mat4(T_array);
	
	view_mat = BT_mat * T_mat;
	
    // Matriz Projection //////////////////////////////////////////////////////
    // You will have to change the contents of this matrix for the exercises
    float proj_array[16] = {1.0f, 0.0f, 0.0f, 0.0f, 
                            0.0f, 1.0f, 0.0f, 0.0f, 
                            0.0f, 0.0f, 1.0f, 0.0f, 
                            0.0f, 0.0f, 0.0f, 1.0f};

    // Parameters #############################################################
    float d = 0.0f;
    //#########################################################################
    
    if(d != 0.0f){
    	proj_array[11] = -1/d;
    	proj_array[14] = d;
		proj_array[15] = 0.0f;
	}
	
    glm::mat4 proj_mat = glm::make_mat4(proj_array);

    // Thr NDC is a left handed system, so we flip along the Z axis.
    // IMPORTANT: Do not change the contents of this matrix!!!!!!
    float flip_z_array[16] = {1.0f, 0.0f,  0.0f, 0.0f, 
                              0.0f, 1.0f,  0.0f, 0.0f, 
                              0.0f, 0.0f, -1.0f, 0.0f, 
                              0.0f, 0.0f,  0.0f, 1.0f};
    glm::mat4 flip_z_mat = glm::make_mat4(flip_z_array);

    // Matriz ModelViewProjection /////////////////////////////////////////////
    glm::mat4 model_view_proj_mat = flip_z_mat * proj_mat * view_mat * model_mat;

    unsigned int transformLoc;
    GL_CHECK(transformLoc = glGetUniformLocation(shader_program, "transform"));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_view_proj_mat));

    // Ativa o Vertex Array Object selecionado.
    glBindVertexArray(vao);

    // Desenha as tres primeiras primitivias, comecando pela de indice 0.
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glFlush();            //
    glutSwapBuffers();    //
    glutPostRedisplay();  //
}

//********************************************************************************************************************
void ExitProg(void) {
    if (vertex_shader_source) {
        free(vertex_shader_source);
        vertex_shader_source = NULL;
    }

    if (frag_shader_source) {
        free(frag_shader_source);
        frag_shader_source = NULL;
    }

    printf("Exiting...\n");
}

//********************************************************************************************************************
int main(int argc, char** argv) {
    // Inicializa a GLUT
    glutInit(&argc, argv);

    // Cria um color buffer onde cada pixel é representado por 4 bytes (RGBA)
    // Cria um depth buffer (para resolver a oclusão)
    // Cria dois color buffers para reduzir o flickering
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    // Define as dimensões do color buffer (ou a área útil do OpenGL na janela)
    glutInitWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);

    // Posição do canto superior esquerdo da janela OpenGL em relação a tela do computador.
    glutInitWindowPosition(100, 100);

    // Título da janela
    glutCreateWindow("Modern OpenGL: Assignment 3");

    // Load the OpenGL extensions
    GLenum err = glewInit();
    if (GLEW_OK != err) {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      return EXIT_FAILURE; 
    }

    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    // Carrega o codigo fonte do Vertex shader
    LoadShader("vertex_shader.glsl", &vertex_shader_source);

    printf("%s\n", vertex_shader_source);

    unsigned int vertex_shader;

    // Cria um identificador para o Vertex Shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    // Vincula o código fonte do Vertex Shader ao seu identificador
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);

    // Compila dinamicamente (em tempo de execucao) o Vertex Shader
    glCompileShader(vertex_shader);

    // Imprime eventuais mensagens de erro de compilacao do Vertex Shader
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", info_log);
    }

    // Carrega o codigo fonte do Fragment shader
    LoadShader("fragment_shader.glsl", &frag_shader_source);

    printf("%s\n", frag_shader_source);

    unsigned int fragment_shader;

    // Cria um identificador para o Fragment Shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Vincula o código fonte do Fragment Shader ao seu identificador
    glShaderSource(fragment_shader, 1, &frag_shader_source, NULL);

    // Compila dinamicamente (em tempo de execucao) o Fragment Shader
    glCompileShader(fragment_shader);

    // Imprime eventuais mensagens de erro de compilacao do Fragment Shader
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", info_log);
    }

    // Cria um identificador para um Shader program (vertex + fragment shader)
    shader_program = glCreateProgram();

    // Vincula os Fragment e Vertex Shaders ao Program Shader
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // Linka o Fragment e Vertex Shader para formarem o Program Shader 
    glLinkProgram(shader_program);

    // Imprime eventuais mensagens de erro de linkagem do Program Shader
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }

    // Deleta os Fragment e Vertex Shaders, já que eles já foram incorporados 
    // ao Program Shader e não são mais necessários.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Ativa o Vertex Array Object (VAO)
    glBindVertexArray(vao);

    // Cria um novo identificador de buffer 
    glGenBuffers(1, &vbo);

    // Vincula o buffer criado a um Vertex Buffer Object (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Carrega as propriedades (coordenadas + cores) dos vértices no VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributo 'posição' do vértice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributo 'cor' do vértice
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Define a cor a ser utilizada para limpar o color buffer a cada novo frame
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);            

    // Habilita o teste de profundidade (oclusão).
    GL_CHECK(glEnable(GL_DEPTH_TEST));

    atexit(ExitProg);          // deifne o callback de saída do programa
    glutDisplayFunc(Display);  // define o callback que renderizará cada frame

    // Framebuffer scan loop.
    glutMainLoop();

    return EXIT_SUCCESS;
}
