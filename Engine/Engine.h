#pragma once
#include <string>
#include <vector>
#include <iostream>

struct Task {
    std::string description;
    bool completed = false;
    std::string category;
};

class Engine {
public:
    Engine();
    ~Engine();

    bool Initialize(const std::string& title, int width, int height);
    void Run();
    void Shutdown();

    void AddTask(const std::string& desc, const std::string& category);
    void ShowTasks() const;

    bool IsRunning() const { return m_isRunning; }

private:
    bool m_isRunning = false;
    std::vector<Task> m_tasks;
    std::string m_title;
    int m_width = 0;
    int m_height = 0;
};
