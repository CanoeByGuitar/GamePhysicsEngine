//
// Created by 王晨辉 on 2023/5/28.
//

#include "Scene.h"

namespace renderer{
    void Scene::AddObject(const std::shared_ptr<Object>& object) {
        m_objects.push_back(object);
    }
}

