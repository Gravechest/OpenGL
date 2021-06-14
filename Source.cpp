#include <Windows.h>
#include <chrono>
#include <random>
#include <vector>
#include "glew.h"
#include "global.h" 
#include <gl/GL.h>
float player = 0.0;
PAINTSTRUCT verf;
unsigned int VBO;
float time2 = 1.0;
POINT pointer;
std::vector<float> verticles = {0.0,0.0};
void square(float posx,float posy) {
    verticles.push_back(posx - 0.05);
    verticles.push_back(posy - 0.05);
    verticles.push_back(posx + 0.05);
    verticles.push_back(posy - 0.05);
    verticles.push_back(posx - 0.05);
    verticles.push_back(posy + 0.05);
    verticles.push_back(posx + 0.05);
    verticles.push_back(posy + 0.05);
    verticles.push_back(posx + 0.05);
    verticles.push_back(posy - 0.05);
    verticles.push_back(posx - 0.05);
    verticles.push_back(posy + 0.05);
}
int random(int x) {
    std::uniform_int_distribution<int> dist(0, x);
    int time = std::chrono::high_resolution_clock().now().time_since_epoch().count();
    std::default_random_engine rand(time);
    int randf = dist(rand);
    return randf;
}
unsigned int init_game(HWND app) {  
    unsigned int shaderprogram = glCreateProgram();
    glCreateBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), &verticles[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    unsigned int vertexshader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragshader = glCreateShader(GL_FRAGMENT_SHADER);
    glUniform1f(glGetUniformLocation(shaderprogram, "time"), time2);
    glUniform1f(glGetUniformLocation(shaderprogram, "playerpos"), player);
    glShaderSource(vertexshader, 1, &vertexshaderSource, NULL);
    glShaderSource(fragshader, 1, &fragshaderSource, NULL);
    glCompileShader(vertexshader);      
    glCompileShader(fragshader);
    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragshader);
    glLinkProgram(shaderprogram);
    glUseProgram(shaderprogram);
    return shaderprogram;
}
void the_game(HDC pixel,unsigned int shaderprogram,HWND app) {
    if (GetKeyState(0x41) < 0) {
        player += 0.1;
    }
    if (GetKeyState(0x44) < 0) {
    }
    if (GetKeyState(0x53) < 0) {
        player -= 0.1;
    }
    if (GetKeyState(0x57) < 0) {
    }
    if (GetKeyState(VK_LBUTTON) < 0) {
        GetCursorPos(&pointer);
        ScreenToClient(app, &pointer);
        float xcoord = (float)pointer.x / 500 - 1.0;
        float ycoord = 1.0 - (float)pointer.y / 480;
        xcoord = round(xcoord * 10) / 10;
        ycoord = round(ycoord * 10) / 10;
        square(xcoord, ycoord);
        glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), &verticles[0], GL_STATIC_DRAW);
    }
    time2 += 0.01;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 1, verticles.size());
}
