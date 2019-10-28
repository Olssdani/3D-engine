#include "Render.h"


Render::Render() {
	Init();
}


bool Render::Init() {
	
	// glfw: initialize and configure
	// ------------------------------
	glfwSetErrorCallback(glfw_error_callback);
	
	if (!glfwInit())
		return false;

	// OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Compatibility for apple computers
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

	//Get primary monitor information
	primaryMonitor = glfwGetPrimaryMonitor();
	primaryVidMode = glfwGetVideoMode(primaryMonitor);
	// glfw window creation
	// --------------------
	#if defined(WINDOW_MODE_EXPLICIT)
		window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_WIDTH, "Engine", NULL, NULL);
	#elif defined(WINDOW_MODE_FULLSCREEN)
		window = glfwCreateWindow(primaryVidMode->width, primaryVidMode->width, "Engine", primaryMonitor, NULL);
	#elif defined(WINDOW_MODE_FULLSCREEN_WINDOWED)
		window = glfwCreateWindow(primaryVidMode->width, primaryVidMode->width, "Engine", NULL, NULL);
	#endif 
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	InitCallbackFunctions();

	// Disable the mouse and capture it
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Initialize Camera
	editorCamera = new Editor_Camera(glm::vec3(0.0f, 0.0f, 10.0f));
	//Attach the camera to the window pointer for the scroll wheel callback
	glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));
	//Intialize GUI
	gui = new GUI(window, this);

	//Initalize Input
	input = new Input(window);

	scene = new Scene();
}

void Render::InitCallbackFunctions() {
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
}



void Render::Rendering() {
	//Bakground color
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!glfwWindowShouldClose(window))
	{
		/*
			Update critical classes first
		*/
		input->update();
		time.update();
		mouse_callback();
		glfwGetWindowSize(window, &width, &height);
		//Evaluate inputs, must be done after input update!!!!
		processEditorInputs(window);
		editorCamera->processInput(input, xoffset, yoffset);

		/*
			RENDERING
		*/
		//Clear screen
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		//Get the current projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(editorCamera->getFov()), (float)width / (float)height, 0.1f, 1000.0f);
		//Get the current view matrix;
		glm::mat4 view = editorCamera->View();
		//Render the scene
		scene->renderScene(projection, view, editorCamera->GetPosition());
		//Render GUI
		gui->guiRender();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}


void Render::processEditorInputs(GLFWwindow *window)
{
	//Toggle wireframe or solid
	if (input->getKeyStatus(KEY_1))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (input->getKeyStatus(KEY_2))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
}

/*
	CallbackFunctions
*/

//Get mouse movement
void Render::mouse_callback(){


	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	xoffset = xpos - lastX;
	yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

}

Editor_Camera* Render::getCamera() {
	return editorCamera;
}
