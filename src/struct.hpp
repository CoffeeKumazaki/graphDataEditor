#pragma once

struct Node {
  double x, y;
  std::list<std::shared_ptr<Node>> child;
};

using NODE_PRT = std::shared_ptr<Node>;
using ND_IT = std::list<NODE_PRT>::iterator;
using ND_CIT = std::list<NODE_PRT>::const_iterator;

struct Edge {
  uint32_t id;
  uint32_t nid[2];
  Edge(){
  };
};

using EDGE_PRT = std::shared_ptr<Edge>;
using EG_IT = std::list<EDGE_PRT>::iterator;
using EG_CIT = std::list<EDGE_PRT>::const_iterator;
