#include "pch.h"
#include "Engine.h"
//#include "GameObject.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Engine::Engine()
{
}

Engine::~Engine()
{
    Shutdown();
}

bool Engine::Initialize(const std::string& title, int width, int height)
{
    m_title = title;
    m_width = width;
    m_height = height;
    m_isRunning = true;
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "   ROGALIQUE GAME ENGINE v1.0" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Game: " << m_title << std::endl;
    std::cout << "Resolution: " << m_width << "x" << m_height << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    return true;
}

void Engine::AddTask(const std::string& desc, const std::string& category)
{
    Task task;
    task.description = desc;
    task.completed = false;
    task.category = category;
    m_tasks.push_back(task);
    AddTask("Создать игрока", "Геймплей");
    AddTask("Реализовать движение", "Геймплей");
    AddTask("Добавить врагов", "Контент");
    
    std::cout << "[NEW TASK] " << category << ": " << desc << std::endl;
}

void Engine::ShowTasks() const
{
    std::cout << "\nCURRENT TASKS:" << std::endl;
    std::cout << "========================================" << std::endl;
    
    for (const auto& task : m_tasks)
    {
        if (!task.completed)
        {
            std::cout << "  [ ] " << task.category << ": " << task.description << std::endl;
        }
    }
    
    std::cout << "========================================\n" << std::endl;
}

void Engine::Run()
{
    std::cout << "\nStarting game loop..." << std::endl;
    ShowTasks();
    std::cout << "Game is running. Press Enter to exit..." << std::endl;
    std::cin.get();
}

void Engine::Shutdown()
{
    std::cout << "\nShutting down Rogalique Engine...\n" << std::endl;
    m_isRunning = false;
}

void Engine::ProcessInput() {}
void Engine::Update(float deltaTime) {}
void Engine::Render() {}
void Engine::CalculateDeltaTime() {}
void Engine::CompleteTask(const std::string& desc) {}
void Engine::ShowProgress() const {}
void Engine::AddGameObject(GameObject* obj) {}
void Engine::RemoveGameObject(GameObject* obj) {}
void Engine::UpdateAll(float deltaTime) {}
void Engine::RenderAll() {}
void Engine::PrintWelcomeMessage() const {}
