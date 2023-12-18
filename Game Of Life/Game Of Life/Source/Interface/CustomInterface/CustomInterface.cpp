#include "CustomInterface.h"

#include <chrono>
#include <thread>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "UpdateType.h"
#include "Settings/Settings.h"
#include "FpsLimiter/FpsLimiter.h"
#include "FpsCounter/FpsCounter.h"
#include "WindowTitle/WindowTitle.h"
#include "ImGui/ImGuiHandler/ImGuiHandler.h"

CustomInterface::CustomInterface(int width, int height, string title)
{
	this->width = width;
	this->height = height;
	this->title = new WindowTitle(title);

	glfwInit();
	SetVersion(4, 6);
	CreateWindow();
	InitializeGlad();
	CreateFpsHandlers();
	Settings::gui = this;
	ImGuiHandler::Initialize(window);
	
	glfwSetFramebufferSizeCallback(window, ResizeCallback);

	// Create the OpenGL window
	glViewport(0, 0, width, height);
}

void CustomInterface::SetVersion(char major, char minor) const
{
	// Set OpenGL major (x.) and minor (.x) version 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

	// Use core (non-deprecated) OpenGL functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void CustomInterface::CreateWindow()
{
	// Create the GLFW window
	window = glfwCreateWindow(width, height, title->ToString().c_str(), NULL, NULL);
	
	if (window == NULL)
	{
		cout << "GLFW window creation error" << endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // Disable V-Sync
}

void CustomInterface::InitializeGlad()
{
	// Load GLAD and let it configure OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "GLAD initialization error" << endl;
	}
}

void CustomInterface::CreateFpsHandlers()
{
	Settings::TargetFps = float(glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);
	gameFpsLimiter = new FpsLimiter(Settings::TargetFps);
	uiFpsLimiter = new FpsLimiter(Settings::TargetFps);
	gameFpsCounter = new FpsCounter(10);
	uiFpsCounter = new FpsCounter(2);
}

void CustomInterface::ResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool CustomInterface::ShouldExit() const
{
	return glfwWindowShouldClose(window);
}

UpdateType CustomInterface::Update()
{
	UpdateType updateType = None;

	if (Settings::ThreadSleep)
		this_thread::sleep_for(chrono::milliseconds(1));

	if (gameFpsLimiter->Update())
	{
		updateType |= Game;

		if (gameFpsCounter->Update())
			title->SetSubText("Game: " + gameFpsCounter->ToString());
	}

	if (uiFpsLimiter->Update())
	{
		updateType |= Interface;

		if (uiFpsCounter->Update())
			title->GetSubTitle()->SetSubText("Interface: " + uiFpsCounter->ToString());
		
		UpdateTitle();
		glfwPollEvents();
		ImGuiHandler::Render();
		glfwSwapBuffers(window);
	}

	return updateType;
}

void CustomInterface::SetTargetFps(double targetFps) const
{
	gameFpsLimiter->SetTargetFps(targetFps);
}

GLFWwindow* CustomInterface::GetWindow() const
{
	return window;
}

int CustomInterface::GetWidth() const
{
	return width;
}

int CustomInterface::GetHeight() const
{
	return height;
}

WindowTitle* CustomInterface::GetTitle()
{
	return title;
}

void CustomInterface::UpdateTitle() const
{
	if (title->IsOutdated())
	{
		title->Update();
		glfwSetWindowTitle(window, title->ToString().c_str());
	}
}

void CustomInterface::Destroy() const
{
	delete title;
	delete uiFpsLimiter;
	delete uiFpsCounter;
	delete gameFpsLimiter;
	delete gameFpsCounter;

	ImGuiHandler::Destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}
