#define _CRT_SECURE_NO_WARNINGS
#define GLEW_STATIC
#include "GL/glew.h"
#include <cstdio>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h" 
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

int main(int argc,char **argv)
{
	
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwSwapInterval(100);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[] = {
			-0.5f, -0.5f, 0.0f,0.0f,// 0
			 0.5f, -0.5f, 1.0f,0.0f, // 1
			 0.5f, 0.5f,  1.0f,1.0f,// 2
			-0.5f, 0.5f,  0.0f,1.0f, // 3
		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);



		Shader shader("res/shader/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/Reaper.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		shader.Unbind();
		va.Unbind();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();


			shader.Bind();
			shader.SetUniform4f("u_Color",r,0.3f,0.8f,1.0f);

			renderer.Draw(va, ib, shader);
			

			if (r > 1.0f)
				increment = -0.0005f;
			else if (r < 0.0f)
				increment = 0.0005f;

			r += increment;
			/* Swap fronçt and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}
	glfwTerminate();
	return 0;
}

