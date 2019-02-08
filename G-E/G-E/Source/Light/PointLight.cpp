#include "PointLight.h"
#include "Object/Box.h"
PointLight::PointLight(glm::vec3 _Position, float _Constant, float _Linear, float _Quadratic)
{
	Position = _Position;
	Constant = _Constant;
	Linear = _Linear;
	Quadratic = _Quadratic;
	box = new Box(1.0f, 1.0f, 1.0f);
	box->Scale(glm::vec3(0.2, 0.2, 0.2));
	box->Translate(Position);
	box->ChangeShader("Shaders/Light_Vert.glsl", "Shaders/Light_Frag.glsl");

}
Box* PointLight::getBox()
{
	return box;
}
void PointLight::Send2GPU(Shader *shader, unsigned int nr)
{
	shader->setVec3("pointLights[" + std::to_string(nr) + "].position", Position);
	shader->setFloat("pointLights[" + std::to_string(nr) + "].constant", Constant);
	shader->setFloat("pointLights[" + std::to_string(nr) + "].linear", Linear);
	shader->setFloat("pointLights[" + std::to_string(nr) + "].quadratic", Quadratic);
	shader->setVec3("pointLights[" + std::to_string(nr) + "].ambient", Ambient);
	shader->setVec3("pointLights[" + std::to_string(nr) + "].diffuse", Diffuse);
	shader->setVec3("pointLights[" + std::to_string(nr) + "].specular", Specular);
	shader->setBool("pointLights[" + std::to_string(nr) + "].init", true);
}
glm::vec3 PointLight::getDiffuse()
{
	return Diffuse;
}