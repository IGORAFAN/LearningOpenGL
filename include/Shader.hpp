#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
public:
    // Конструктор генерирует шейдер "на лету"
    Shader(const char* vertexPath, const char* fragmentPath);

	// Активация шейдера
    void use() const;

	// Полезные uniform-функции
    void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	[[nodiscard]] unsigned int getID() const { return ID; }

  private:
	unsigned int ID;

    // Полезные функции для проверки ошибок компиляции/связывания шейдеров
    static void checkCompileErrors(unsigned int shader, const std::string& type);
};
#endif