#ifndef ENGINE_H
#define ENGINE_H

#ifdef ENGINE_EXPORTS
    #define ENGINE_API __declspec(dllexport)
#else
    #define ENGINE_API __declspec(dllimport)
#endif

#include <string>
#include <chrono>
#include <vector>
#include <memory>

// Forward declarations
class GameObject;

struct Task {
    std::string description;
    bool completed = false;
    std::string category;
};

class ENGINE_API Engine {
public:
    Engine();
    ~Engine();

    bool Initialize(const std::string& title, int width, int height);
    void Run();
    void Shutdown();

    // Task system
    void AddTask(const std::string& desc, const std::string& category);
    void CompleteTask(const std::string& desc);
    void ShowTasks() const;
    void ShowProgress() const;

    // GameObject management
    void AddGameObject(GameObject* obj);
    void RemoveGameObject(GameObject* obj);
    void UpdateAll(float deltaTime);
    void RenderAll();

    bool IsRunning() const { return m_isRunning; }
    float GetDeltaTime() const { return m_deltaTime; }

private:
    void ProcessInput();
    void Update(float deltaTime);
    void Render();
    void CalculateDeltaTime();
    void PrintWelcomeMessage() const;

    bool m_isRunning = false;
    float m_deltaTime = 0.0f;
    std::chrono::steady_clock::time_point m_lastTime;
    std::string m_title;
    int m_width = 0;
    int m_height = 0;
    std::vector<Task> m_tasks;
    std::vector<GameObject*> m_gameObjects;
};

#endif // ENGINE_H
