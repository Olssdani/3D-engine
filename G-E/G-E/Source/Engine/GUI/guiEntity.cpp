#include "guiEntity.h"
#include "Misc/windowsUtil.h"
#include "Object/box.h"
#include "Object/model.h"
#include "Scene/scene.h"
#include "Light/directionalLight.h"
#include "Light/pointLight.h"
#include "Render/render.h"
#include "Misc/Utilities.h"
#include <stdio.h>
#include <stdlib.h>

guiEntity::guiEntity(GLFWwindow* w, render* r) {
	window = w;
	intialize();
	renderObject = r;

	addPosition[0] = 0;
	addPosition[1] = 0;
	addPosition[2] = 0;
}

void guiEntity::intialize() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsLight();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}
//TODO fix virtual function
void guiEntity::guiRender() {}

void guiEntity::guiRender(const unsigned int editorTexture,
						  const unsigned int gameTexture,
						  const unsigned int viewportWidth,
						  const unsigned int viewportHeight,
						  const unsigned int textureWidth,
						  const unsigned int textureHeight) {
	static bool no_titlebar = true;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = true;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;

	ImGuiWindowFlags window_flags = 0;
	if(no_titlebar)
		window_flags |= ImGuiWindowFlags_NoTitleBar;
	if(no_scrollbar)
		window_flags |= ImGuiWindowFlags_NoScrollbar;
	if(!no_menu)
		window_flags |= ImGuiWindowFlags_MenuBar;
	if(no_move)
		window_flags |= ImGuiWindowFlags_NoMove;
	if(no_resize)
		window_flags |= ImGuiWindowFlags_NoResize;
	if(no_collapse)
		window_flags |= ImGuiWindowFlags_NoCollapse;
	if(no_nav)
		window_flags |= ImGuiWindowFlags_NoNav;
	if(no_background)
		window_flags |= ImGuiWindowFlags_NoBackground;
	if(no_bring_to_front)
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	//if (no_close)           p_open = NULL; // Don't pass our bool* to Begin
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(viewportWidth, viewportHeight), ImGuiCond_Once);
		ImGui::Begin("Scene",
					 NULL,
					 window_flags); // Create a window called "Hello, world!" and append into it.
		if(ImGui::BeginMenuBar()) {
			if(ImGui::BeginMenu("Menu")) {
				ImGui::MenuItem("New(Placeholder)");
				ImGui::MenuItem("Save(Placeholder)");
				ImGui::MenuItem("Open(Placeholder)");
				ImGui::EndMenu();
			}
			ImGui::MenuItem("Edit");
			ImGui::EndMenuBar();
		}

		// left
		ImGui::BeginChild("left panel", ImVec2(viewportWidth * 0.15, 200), false);

		ImGui::EndChild();
		ImGui::SameLine();

		// right
		ImGui::BeginChild("item view", ImVec2(textureWidth, 0));
		{
			if(ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
				if(ImGui::BeginTabItem("Editor")) {
					cameraActiv = cameraType::EDITOR;
					ImGui::Image((void*)(intptr_t)editorTexture,
								 ImVec2(textureWidth, textureHeight));
					ImGui::EndTabItem();
				}
				if(ImGui::BeginTabItem("Game")) {
					cameraActiv = cameraType::MAIN;
					ImGui::Image((void*)(intptr_t)gameTexture, ImVec2(textureWidth, textureHeight));
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::Separator();
		ImGui::EndChild();
		ImGui::NextColumn();
		ImGui::SameLine();

		ImGui::BeginChild("Scene graph", ImVec2(200, 0)); // Leave room for 1 line below us
		{
			if(ImGui::Button("Add"))
				ImGui::OpenPopup("Popup_Add");

			if(ImGui::BeginPopup("Popup_Add")) {
				if(ImGui::BeginMenu("Object")) {
					if(ImGui::Selectable("Box")) {
						box* b = new box();
						const char default[ 5 ] = "Box";
						b->setName(default + std::to_string(b->getID()));
						renderObject->getScene()->addObject(b);
					}
					if(ImGui::Selectable("Load object")) {
						std::string fileName = wUtil::openFileExplorer();
						if(fileName.compare("")) {
							std::cout << "load";
							model* m = new model(fileName);
							const char default[ 6 ] = "Model";
							m->setName("Model");
							m->setName(default + std::to_string(m->getID()));
							renderObject->getScene()->addObject(m);
						} else {
							std::cout << "Not load";
						}
					}
					ImGui::EndMenu();
				}
				if(ImGui::BeginMenu("Lights")) {
					if(ImGui::Selectable("Point Light")) {
						pointLight* l = new pointLight(glm::vec3(0, 0, 0));
						l->setName("Point Light " + std::to_string(l->getID()));
						renderObject->getScene()->addPointLight(l);
					}
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}
			ImGui::Separator();

			directionalLight* dl = renderObject->getScene()->getDirectionalLight();
			if(ImGui::Selectable(dl->getName().c_str(), selectedClass == 0)) {
				selectedClass = 0;
			}
			std::vector<pointLight*> lights = renderObject->getScene()->getPointLights();
			for(int i = 0; i < lights.size(); ++i) {
				if(ImGui::Selectable(lights[i]->getName().c_str(),
									 selectedItem == i && selectedClass == 1)) {
					selectedClass = 1;
					selectedItem = i;
				}
			}

			std::vector<object*> objects = renderObject->getScene()->getObjectList();
			for(int i = 0; i < objects.size(); ++i) {
				if(ImGui::Selectable(objects[i]->getName().c_str(),
									 selectedItem == i && selectedClass == 2)) {
					selectedClass = 2;
					selectedItem = i;
				}
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Details", ImVec2(0, 800), true);
		{
			ImGui::Text("Entity Properties");
			if(selectedClass == 0) {
				renderObject->getScene()->getDirectionalLight()->guiRender();
			} else if(selectedClass == 1) {
				renderObject->getScene()->getPointLights()[selectedItem]->guiRender();
			} else if(selectedClass == 2) {
				renderObject->getScene()->getObjectList()[selectedItem]->guiRender();
			}
		}
		ImGui::EndChild();
		ImGui::End();
	}
	//ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

guiEntity::cameraType guiEntity::activeCamera() {
	return cameraActiv;
}

void guiEntity::changeEditor(cameraType c) {
	cameraActiv = c;
}
