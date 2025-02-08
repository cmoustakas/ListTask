#pragma once

#include <list>
#include <string>
constexpr size_t kInvalidSz = 0xFFFFFFFF;

struct Entity;
using ListOfEntities = std::list<Entity>;

struct Entity {
  std::string m_name = "";
  int m_count = kInvalidSz;
  int m_strength = kInvalidSz;
};
