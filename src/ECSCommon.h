#pragma once

#include <cstdint>
#include <bitset>

using EntityHandle = std::uint32_t;
using ComponentType = std::uint8_t;

static const std::uint32_t MAX_ENTITIES = 5000;
static const std::uint8_t MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;
