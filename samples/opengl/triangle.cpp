
#include <stdio.h>
#include <signal.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../shader.h"
#define CIXEL_IMPLEMENTATION
#include "cixel.h"

const char VertexShader[] =
"#version 330 core\n"
"layout(location=0) in vec3 vertexPosition_modelspace;"
"uniform mat4 MVP;"
"void main(){"
"vec4 pos = vec4(vertexPosition_modelspace, 1);"
"gl_Position = MVP * pos;"
"}"
;

const char FragmentShader[] =
"#version 330 core\n"
"out vec3 color;"
"void main(){"
"color = vec3(1,0,0);"
"}"
;

void quantizeYUV655(cixel::cixel_u32* pixels, cixel::cixel_s32 size, const cixel::cixel_u8* indices, const cixel::Cixel* cixel)
{
    for(int i = 0; i < size; ++i) {
        cixel::Color color = cixel::cixelGetPalletColor(cixel, indices[i]);
        color.color_ = cixel::cixelYUV2RGB(color.color_);

        cixel::cixel_s8 y = static_cast<cixel::cixel_s8>(color.rgba_.r_);
        cixel::cixel_s8 u = static_cast<cixel::cixel_s8>(color.rgba_.g_);
        cixel::cixel_s8 v = static_cast<cixel::cixel_s8>(color.rgba_.b_);
        cixel::Color c;
        c.rgba_.a_ = 0xFFU;
        c.rgba_.r_ = y;
        c.rgba_.g_ = u;
        c.rgba_.b_ = v;
        pixels[i] = c.color_;
    }
}

static bool signaled_ = false;
static void onSignal(int sig)
{
    signaled_ = true;
}

int main(int argc, char** argv)
{
    static const int Width = 400;
    static const int Height = 300;

    if(SIG_ERR == signal(SIGINT, onSignal)){
        return -1;
    }

    if(!glfwInit()){
        fprintf(stderr, "Error: failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(400, 300, "", NULL, NULL);
    if(NULL == window){
        fprintf(stderr, "Error: failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if(GLEW_OK != glewInit()){
        fprintf(stderr, "Error: failed to initialize GLEW\n");
        return -1;
    }

    //Create render buffer
    GLuint renderBufferID;
    glGenRenderbuffersEXT(1, &renderBufferID);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBufferID);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, 400, 300);

    //Create frame buffer
    GLuint frameBufferID;
    glGenFramebuffersEXT(1, &frameBufferID);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID);

    //Bind render buffer to frame buffer
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, renderBufferID);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);// Dark blue background

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    GLuint programID = createShaders(VertexShader, FragmentShader);

    static const GLfloat vertexBufferData[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

    cixel::Cixel* cixel = cixel::cixelCreate(Width, Height, CIXEL_NULL, CIXEL_NULL);

    static const int size = Width*Height;
    cixel::cixel_u32* buffer = reinterpret_cast<cixel::cixel_u32*>(malloc(size*4));
    cixel::cixel_u8* indices = reinterpret_cast<cixel::cixel_u8*>(malloc(size * sizeof(cixel::cixel_u8)));

    // Get a handle for our "MVP" uniform
	GLuint matrixID = glGetUniformLocation(programID, "MVP");
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 view = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 MVP;
    glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);

    printf("\033[H\0337"); //Save current cursor's position
    int roty = 0;
    do{
        printf("\0338");
        fflush(stdout);

        if((360*8)<=++roty){
            roty = 0;
            model = glm::mat4(1.0f);
        }else{
            model = glm::rotate(model, 0.125f, rotationAxis);
        }
        MVP = projection * view * model;

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // attribute 0
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized
            0,        // stride
            (void*)0  // array buffer offset
        );
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
        glReadPixels(0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        cixel::cixelQuantize(cixel, indices, buffer, true);
        cixel::cixelPrint(cixel, stdout, indices);

        //glfwSwapBuffers(window);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glfwPollEvents();
    }while(!signaled_);
    //}while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && 0 == glfwWindowShouldClose(window));

    free(indices);
    free(buffer);
    cixel::cixelDestroy(cixel);
    glDeleteFramebuffersEXT(1, &frameBufferID);
    glDeleteRenderbuffersEXT(1, &renderBufferID);
    printf("\033[2J");
    return 0;
}

