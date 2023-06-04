//
// Created by 王晨辉 on 2023/5/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_SCENE_H
#define GAMEPHYSICSINONEWEEKEND_SCENE_H

#include "Objects/ObectBase.h"



namespace renderer{
class Scene{
public:
    void AddObject(const std::shared_ptr<Object>& object);
    std::vector<std::shared_ptr<Object>>& GetObjects(){
        return this->m_objects;
    }

private:
    std::vector<std::shared_ptr<Object>> m_objects;
};

}


#endif //GAMEPHYSICSINONEWEEKEND_SCENE_H
