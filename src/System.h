#pragma once

#include <set>

#include "ECSCommon.h"

class System {
public:
   std::set<EntityHandle> m_entities;
};
