#include "CustomInterface.h"

#include <chrono>
#include <thread>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "UpdateType.h"
#include "Settings/LogString/LogString.h"
#include "Settings/Settings.h"
#include "Inputs/InputHandler.h"
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
	ActivateCallbacks();
	CreateFpsHandlers();

	Settings::gui = this;
	inputHandler = new InputHandler();
	imGuiHandler = new ImGuiHandler(window);

	// Create the OpenGL window
	ResizeCallback(window, width, height);
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
		Settings::log << "GLFW window creation error" << "\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // Disable V-Sync

	// Move window to center
	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(
		window, (videoMode->width / 2) - (width / 2),
		(videoMode->height / 2) - (height / 2)
	);
}

void CustomInterface::InitializeGlad()
{
	// Load GLAD and let it configure OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Settings::log << "GLAD initialization error" << "\n";
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
		{
			metrics[0] = gameFpsCounter->GetFps();
			metrics[1] = gameFpsCounter->GetFrametime();
			title->SetSubText("Simulation: " + gameFpsCounter->ToString());
		}
	}

	if (uiFpsLimiter->Update())
	{
		updateType |= Interface;

		if (uiFpsCounter->Update())
		{
			metrics[2] = uiFpsCounter->GetFps();
			metrics[3] = uiFpsCounter->GetFrametime();
			title->GetSubTitle()->SetSubText("Interface: " + uiFpsCounter->ToString());
		}

		UpdateTitle();
		glfwPollEvents();
		inputHandler->Update();
		imGuiHandler->Render();
		glfwSwapBuffers(window);
	}

	return updateType;
}

void CustomInterface::ApplyFullscreenState() const
{
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

	if (Settings::FullscreenEnabled)
	{
		glfwSetWindowMonitor(
			window, primaryMonitor, 0, 0, videoMode->width,
			videoMode->height, GLFW_DONT_CARE
		);
	}
	else
	{
		glfwSetWindowMonitor(
			window, NULL, (videoMode->width / 2) - (width / 2),
			(videoMode->height / 2) - (height / 2), width, height, GLFW_DONT_CARE
		);
	}
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

const double* CustomInterface::GetMetrics() const
{
	return metrics;
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

CustomInterface::~CustomInterface()
{
	delete title;
	delete inputHandler;
	delete uiFpsLimiter;
	delete uiFpsCounter;
	delete gameFpsLimiter;
	delete gameFpsCounter;

	delete imGuiHandler;
	glfwDestroyWindow(window);
	glfwTerminate();
}
