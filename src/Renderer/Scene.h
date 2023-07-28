//
// Created by 王晨辉 on 2023/5/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_SCENE_H
#define GAMEPHYSICSINONEWEEKEND_SCENE_H

#include "../Actor.h"
#include "Objects/ObectBase.h"


namespace renderer {
class Scene {
public:
  void                                  Clear() { m_objects.clear(); }
  void                                  AddObject(const std::shared_ptr<Object>& object);
  std::vector<std::shared_ptr<Object>>& GetObjects() { return this->m_objects; }

  void                                  ReadSceneFromActorVector(const std::vector<Actor*>& world) {
    for (auto& actor : world) {
      for (const auto& obj : actor->m_renderComponent->objects) {
        AddObject(obj);
      }
    }
  }

  void UpdateSceneFromActorVector(const std::vector<Actor*>& world){
    // has bug
    m_objects.clear();
    for (auto& actor : world) {
      for (const auto& obj : actor->m_renderComponent->objects) {
        AddObject(obj);
      }
    }
  }


private:
  std::vector<std::shared_ptr<Object>> m_objects;
};

}   // namespace renderer


#endif   // GAMEPHYSICSINONEWEEKEND_SCENE_H
