#include "Shader.hpp"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Константы
constexpr auto SCR_WIDTH  = 800;
constexpr auto SCR_HEIGHT = 600;

constexpr auto VERTEX_SHADER_PATH	= "S:/Projects/OpenGL/OpenGL_12.05.25/resources/shadersSources/vertexShader.vs";
constexpr auto FRAGMENT_SHADER_PATH = "S:/Projects/OpenGL/OpenGL_12.05.25/resources/shadersSources/fragmentShader.fs";
constexpr auto TEXTURE_PATH1		= "S:/Projects/OpenGL/OpenGL_12.05.25/resources/textures/wooden_container.jpg";
constexpr auto TEXTURE_PATH2		= "S:/Projects/OpenGL/OpenGL_12.05.25/resources/textures/awesomeface.png";

int main()
{
	// glfw: инициализация и конфигурирование
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
#endif

	// glfw: создание окна
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: загрузка всех указателей на OpenGL-функции
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Компилирование нашей шейдерной программы
	const Shader shaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	// Указание вершин (и буфера(ов)) и настройка вершинных атрибутов
	constexpr float vertices[] = {
		// координаты        // цвета            // текстурные координаты
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // верхняя правая вершина
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // нижняя правая вершина
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // нижняя левая вершина
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // верхняя левая вершина
   };

	constexpr unsigned int indices[] = {
		0, 1, 3, // первый треугольник
		1, 2, 3  // второй треугольник
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// Цветовые атрибуты
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Атрибуты текстурных координат
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Загрузка и создание текстуры
	unsigned int texture1, texture2;

	// Текстура №1 - Деревянный ящик
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Установка параметров наложения текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // установка метода наложения текстуры GL_REPEAT
																  // (стандартный метод наложения)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Установка параметров фильтрации текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Загрузка изображения, создание текстуры и генерирование мипмап-уровней
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // указываем stb_image.h на то, чтобы перевернуть для загруженной текстуры
											// ось y
	unsigned char* data = stbi_load(TEXTURE_PATH1, &width, &height, &nrChannels, 0);
	if (data == nullptr)
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	// Текстура №2 - Смайлик
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Установка параметров наложения текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // установка метода наложения текстуры GL_REPEAT
																  // (стандартный метод наложения)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Установка параметров фильтрации текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Загрузка изображения, создание текстуры и генерирование мипмап-уровней
	data = stbi_load(TEXTURE_PATH2, &width, &height, &nrChannels, 0);
	if (data == nullptr)
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Файл awesomeface.png имеет альфа-канал (прозрачность), поэтому необходимо использовать пераметр GL_RGBA
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	// Указываем OpenGL, какой сэмплер к какому текстурному блоку принадлежит (это нужно сделать единожды)
	shaderProgram.use(); // не забудьте активировать шейдер перед настройкой uniform-переменных!
	// Устанавливаем вручную…
	//glUniform1i(glGetUniformLocation(shaderProgram.getID(), "texture1"), 0);
	// …или с помощью шейдерного класса
	shaderProgram.setInt("texture1", 0);
	shaderProgram.setInt("texture2", 1);

	// Цикл рендеринга
	while (!glfwWindowShouldClose(window))
	{
		// Обработка ввода
		processInput(window);

		// Рендеринг
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Привязка текстур к соответствующим текстурным юнитам
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Рендеринг ящика
		shaderProgram.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена
		// кнопка, перемещен курсор мыши и т.п.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: завершение, освобождение всех выделенных ранее GLFW-реурсов
	glfwTerminate();
	return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка
// данных событий
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная
// callback-функция
void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	// Убеждаемся, что окно просмотра соответствует новым размерам окна.
	// Обратите внимание, что высота и ширина будут значительно больше, чем указано, на Retina-дисплеях
	glViewport(0, 0, width, height);
}