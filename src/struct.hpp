#pragma once

struct Node {
  uint32_t id;
  double x, y;
  std::list<std::shared_ptr<Node>> child;
};

using NODE_PRT = std::shared_ptr<Node>;
using ND_IT = std::list<NODE_PRT>::iterator;
using ND_CIT = std::list<NODE_PRT>::const_iterator;
