#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
//#include "Logger.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Explorative.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>


// rotation conrol of planet ( Relocate this)
void rotate(glm::mat4& old_rotation, float& r, float& increment) {
    if (GetKeyState('A') & 0x8000) // rotates around y axis relativ to planet
    {
        old_rotation = glm::rotate(old_rotation, -increment, glm::highp_vec3(0, 1, 0));
    }
    if (GetKeyState('D') & 0x8000)
    {
        old_rotation = glm::rotate(old_rotation, increment, glm::highp_vec3(0, 1, 0));
    }
    if (GetKeyState('W') & 0x8000) // rotates around x axis reltive to camera
    {
        if (r <= M_PI / 2 - 0.05f) {
            r += increment;
            old_rotation = glm::rotate(old_rotation, -increment, glm::highp_vec3(old_rotation[0][0], old_rotation[1][0], old_rotation[2][0]));
        }
    }
    if (GetKeyState('S') & 0x8000)
    {
        if (r >= -M_PI / 2 + 0.05f) {
            r -= increment;
            old_rotation = glm::rotate(old_rotation, increment, glm::highp_vec3(old_rotation[0][0], old_rotation[1][0], old_rotation[2][0]));
        }
    }
}



// zoom control relative to planet surface (not ready)
void zoom(glm::mat4& model,float& z, float& increment2) {
    if (GetKeyState('R') & 0x8000) // rotates around x axis reltive to camera
    {
        if (z <= 10.0f) {
            z += increment2;
            model = glm::translate(model, glm::vec3(0, 0, -increment2));
        }
    }
    if (GetKeyState('F') & 0x8000)
    {
        if (z >=-0.5f) {
            z -= increment2;
            model = glm::translate(model, glm::vec3(0, 0, increment2));
        }
    }
};


void printmatrix(float* data, int m, int n) {
    std::cout << "pos: " << std::endl;
    for (int i = 0; i < n; i = i + 5) {
        std::cout << data[i] << " ";
        std::cout << data[i + 1] << " ";
        std::cout << data[i + 2] << " " << std::endl;
    }
    std::cout << "tex: " << std::endl;
    for (int i = 0; i < n; i = i + 5) {
        std::cout << data[i + 3] << " ";
        std::cout << data[i + 4] << " " << std::endl;
    }
}

void printpoly(float* data, unsigned int* indices, int quads) {
    std::cout << "pos: " << std::endl;
    for (int i = 0; i < quads * 2; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << "("
                << data[indices[i*3+j] * 5] << " " 
                << data[indices[i*3+j] * 5+1] << " "
                << data[indices[i*3+j] * 5+2] << ")" << std::endl;
        }
        
        std::cout << std::endl;
    }
}

int main(void)
{


    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    int window_width = 800;
    int window_height = 600;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(1);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl; // (rolocate)
    {
        float pi = (float)M_PI;
        int model_width = 101;
        int model_height = 51;
        float* positions_planet = new float[model_width * model_height * 5]; // Equirectangular projection^-1 (left to right, down to up)
        float inc_m = 1.0f / ((float)model_height-1);
        float inc_n = 2.0f / ((float)model_width-1);
        for (int j = 0; j < model_height; j++) {
            for (int i = 0; i < model_width; i++) {
                positions_planet[(j * model_width + i) * 5    ] = cos(pi * (-0.5f + j * inc_m)) * sin(pi * (-1.0f + i * inc_n));	//x pos
                positions_planet[(j * model_width + i) * 5 + 2] = cos(pi * (-0.5f + j * inc_m)) * cos(pi * (-1.0f + i * inc_n));	//y pos
                positions_planet[(j * model_width + i) * 5 + 1] = sin(pi * (-0.5f + j * inc_m));								//z pos
                positions_planet[(j * model_width + i) * 5 + 3] = 0.0f + i / ((float)model_width-1);				//x tex 
                positions_planet[(j * model_width + i) * 5 + 4] = 0.0f + j / ((float)model_height-1);				//y tex
            }
        }

        
       // printmatrix(positions_planet, 5, model_width * model_height * 5);


        unsigned int* indices_planet = new unsigned int[(model_width - 1) * (model_height - 1) * 2 * 3]; //(relocate)
        for (int j = 0; j < model_height - 1; j++) {
            for (int i = 0; i < model_width - 1; i++) {
                indices_planet[(j*(model_width-1)+i)*6] = j * model_width + i;			    //1
                indices_planet[(j*(model_width-1)+i)*6+1] = j * model_width + i + 1;		//2
                indices_planet[(j*(model_width-1)+i)*6+2] = (j + 1) * model_width + i;		//3
                indices_planet[(j*(model_width-1)+i)*6+3] = (j + 1) * model_width + i;		//1
                indices_planet[(j*(model_width-1)+i)*6+4] = (j + 1) * model_width + i + 1;	//2
                indices_planet[(j*(model_width-1)+i)*6+5] = j * model_width + i + 1;		//3
            }
        } 

       // printpoly(positions_planet, indices_planet, (model_width - 1) * (model_height - 1));

        

        GLCall(glViewport(0,0,window_width,window_height));
        glMatrixMode(GL_MODELVIEW);
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
        

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));
        
   

        VertexArray va;
        VertexBuffer vb(positions_planet, model_width*model_height*5* sizeof(float)); 
        //VertexBuffer vb(positions2p, 4 * 5 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

       
        IndexBuffer ib(indices_planet, (model_width-1)*(model_height-1)*6); 
        //IndexBuffer ib(indices2, 24);
        glm::mat4 projp = glm::perspective((float)M_PI/4,(float)window_width/(float)window_height,0.1f,100.0f);
        glm::mat4 proj = glm::ortho(0.0f, (float)window_width, 0.0f, (float)window_height, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-3));
        glm::mat4 modelr = glm::mat4(1.0f);

        glm::mat4 mvp = projp *view* model;


        Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 10.f);
        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/1_earth_8k.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture",0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;
        GLCall(glEnable(GL_DEPTH_TEST));
        //glDepthFunc(GL_GREATER);
        float r = 0.0f;
        float increment = 0.05f;
        float increment2 = 0.05f;
        float z = 5.f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.clear();
            shader.Bind();
            //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 10.f);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
            //model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -z));
            zoom(model,z,increment2);
            //modelr = glm::rotate(glm::mat4(1.0f),r,glm::vec3(0,1,0));
            rotate( modelr, r, increment);
            mvp = projp * view * model* modelr;
            shader.SetUniformMat4f("u_MVP", mvp);


            renderer.Draw(va,ib,shader);
           

            
            //if (r > 2 * M_PI) { r -= 2 * M_PI; }
            //if (z >= 10.f|| z <= 0.0) { increment2 = -increment2; }

            //z += increment2;


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }


        delete[] positions_planet;
        delete[] indices_planet;

       
 
    }

    // delete here
    glfwTerminate();
    return 0;
} 