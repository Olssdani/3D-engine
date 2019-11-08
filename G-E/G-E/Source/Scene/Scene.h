#pragma once
#include <vector>
#include "Object/Box.h"
#include "Object/Object.h"
#include "Object/Plane.h"

class Scene {
private:

	//Variables
	std::vector<Object*> objects;
	DirectionalLight DL;
	std::vector<PointLight*> PL;
	Plane *plane;


public:

	Scene()
	{
		plane =new Plane(glm::vec3(0, -5, 0), 100.0f, 100.0f, 1);
		plane->setName("Plane");
		plane->getMaterial()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		plane->getMaterial()->setShininess(32);
		objects.push_back(plane);
		DL = DirectionalLight(glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f));
	}
	void addObject(Object *o) {
		objects.push_back(o);
	}
	void addPointLight(PointLight *l) {
		PL.push_back(l);
	}

	void renderScene(glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition) {
		for each (Object* o in objects)
		{
			o->Render(projection, view, cameraPosition, DL, PL);
		}

		for each (PointLight* p in PL)
		{
			p->getBox()->RenderNoLight(projection, view, cameraPosition);
		}
	}

	std::vector<Object*> getObjectList() {
		return objects;
	}

	std::vector<PointLight*> getPointLights() {
		return PL;
	}

	DirectionalLight getDirectionalLight() {
		return DL;
	}
};