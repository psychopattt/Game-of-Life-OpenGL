#include "Interface.h"

#include <cmath>
#include <chrono>
#include <thread>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Settings/LogString/LogString.h"
#include "Settings/Settings.h"
#include "Settings/UpdateType.h"
#include "Inputs/InputHandler.h"
#include "Simulation/Simulation.h"
#include "FpsLimiter/FpsLimiter.h"
#include "FpsCounter/FpsCounter.h"
#include "WindowTitle/WindowTitle.h"
#include "ImGui/ImGuiHandler/ImGuiHandler.h"

using std::make_unique;

Interface::Interface(int width, int height, string title,
	Simulation* simulation) : width(width), height(height),
	initialWidth(width), initialHeight(height),
	simulation(simulation)
{
	this->title = make_unique<WindowTitle>(title);

	glfwInit();
	SetVersion(4, 6);
	CreateWindow();
	InitializeGlad();
	ActivateCallbacks();
	CreateFpsHandlers();

	Settings::Gui = this;
	inputHandler = make_unique<InputHandler>();
	imGuiHandler = make_unique<ImGuiHandler>(window);

	// Create the OpenGL window
	ResizeCallback(window, width, height);
}

void Interface::SetVersion(char major, char minor) const
{
	// Set OpenGL major (x.) and minor (.x) version 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

	// Use core (non-deprecated) OpenGL functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Interface::CreateWindow()
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

void Interface::InitializeGlad()
{
	// Load GLAD and let it configure OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Settings::log << "GLAD initialization error" << "\n";
	}
}

void Interface::CreateFpsHandlers()
{
	Settings::TargetFps = (float)glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
	gameFpsLimiter = make_unique<FpsLimiter>(Settings::TargetFps);
	uiFpsLimiter = make_unique<FpsLimiter>(Settings::TargetFps);
	gameFpsCounter = make_unique<FpsCounter>(10);
	uiFpsCounter = make_unique<FpsCounter>(2);
}

bool Interface::ShouldExit() const
{
	return glfwWindowShouldClose(window);
}

UpdateType Interface::Update()
{
	UpdateType updateType = None;

	if (Settings::ThreadSleep)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	if (gameFpsLimiter->Update() || stepFrame)
	{
		stepFrame = false;
		updateType |= SimulationUpdate;

		if (gameFpsCounter->Update())
		{
			metrics[0] = gameFpsCounter->GetFps();
			metrics[1] = gameFpsCounter->GetFrametime();
			title->SetSubText("Simulation: " + gameFpsCounter->ToString());
		}
	}

	if (uiFpsLimiter->Update())
	{
		updateType |= DisplayUpdate;

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

void Interface::UpdateTitle() const
{
	if (title->IsOutdated())
	{
		title->Update();
		glfwSetWindowTitle(window, title->ToString().c_str());
	}
}

void Interface::ComputeViewportSettings()
{
	int simWidth = simulation->GetWidth();
	int simHeight = simulation->GetHeight();
	float simAspectRatio = static_cast<float>(simWidth) / simHeight;

	if (width > height * simAspectRatio)
	{
		int viewportHeight = lround(simHeight * static_cast<float>(width) / simWidth);
		int heightOffset = -lround((viewportHeight - height) / 2.0f);
		glViewport(0, heightOffset, width, viewportHeight);
	}
	else
	{
		int viewportWidth = lround(simWidth * static_cast<float>(height) / simHeight);
		int widthOffset = -lround((viewportWidth - width) / 2.0f);
		glViewport(widthOffset, 0, viewportWidth, height);
	}
}

void Interface::ApplyFullscreenState() const
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
			window, NULL, (videoMode->width / 2) - (initialWidth / 2),
			(videoMode->height / 2) - (initialHeight / 2), initialWidth,
			initialHeight, GLFW_DONT_CARE
		);
	}
}

void Interface::SetTargetFps(float targetFps) const
{
	Settings::TargetFps = targetFps < 0 ? 0 : targetFps;
	Settings::ThreadSleep = Settings::TargetFps < 100;
	gameFpsLimiter->SetTargetFps(Settings::TargetFps);
}

void Interface::StepFrame()
{
	if (Settings::TargetFps > 0)
		SetTargetFps(0);

	stepFrame = true;
}

int Interface::GetWidth() const
{
	return width;
}

int Interface::GetHeight() const
{
	return height;
}

void Interface::GetViewportSize(int& width, int& height) const
{
	int viewportSettings[4];
	glGetIntegerv(GL_VIEWPORT, viewportSettings);
	width = viewportSettings[2];
	height = viewportSettings[3];
}

const double* Interface::GetMetrics() const
{
	return metrics;
}

WindowTitle* Interface::GetTitle() const
{
	return title.get();
}

void Interface::GetMousePosition(double* posX, double* posY) const
{
	glfwGetCursorPos(window, posX, posY);
}

GLFWwindow* Interface::GetWindow() const
{
	return window;
}

Interface::~Interface()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
