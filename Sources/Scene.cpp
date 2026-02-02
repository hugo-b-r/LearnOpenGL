#include "Scene.h"
#include "Camera.h"
#include "World/World.h"

Scene::Scene(Configuration &c) : m_config(c), m_cam(*this) {}

// avec une camera précédement initialisée avec la configuration
Scene::Scene(Configuration &c, Camera &cam) : m_config(c), m_cam(cam) {}

// avec une camera précédement initialisée avec la configuration
Scene::Scene(Configuration &c, Camera &cam, World &world)
    : m_config(c), m_cam(cam), m_world(world) {}

Scene::Scene() : m_config(), m_cam(*this), m_world() {}

Camera &Scene::getCamera() { return m_cam; }
Configuration &Scene::getConfig() { return m_config; }

World &Scene::getWorld() { return m_world; }
