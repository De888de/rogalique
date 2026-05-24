#include "Engine.h"

Engine::Engine()
{
    std::cout << "[Engine] Constructor" << std::endl;
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

    std::cout << "[Engine] Initialized: " << m_title << std::endl;
    
    AddTask("Create player", "Gameplay");
    AddTask("Implement movement", "Gameplay");
    AddTask("Add enemies", "Content");
    
    return true;
}

void Engine::AddTask(const std::string& desc, const std::string& category)
{
    Task task{desc, false, category};
    m_tasks.push_back(task);
    std::cout << "[TASK] " << category << ": " << desc << std::endl;
}

void Engine::ShowTasks() const
{
    std::cout << "\n=== CURRENT TASKS ===" << std::endl;
    for (const auto& task : m_tasks)
    {
        if (!task.completed)
        {
            std::cout << "[ ] " << task.category << ": " << task.description << std::endl;
        }
    }
}

void Engine::Run()
{
    std::cout << "[Engine] Running..." << std::endl;
    ShowTasks();
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
}

void Engine::Shutdown()
{
    std::cout << "[Engine] Shutdown" << std::endl;
    m_isRunning = false;
}
