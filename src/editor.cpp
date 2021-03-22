#include <stdafx.hpp>
#include "struct.hpp"
#include "editor.hpp"

GraphDataEditor::GraphDataEditor() {

}

GraphDataEditor::~GraphDataEditor() {

}

void GraphDataEditor::init() {

  nodes.clear();
  edges.clear();
}

void GraphDataEditor::addNode(NODE_PRT pn) {

  if (pn && find(nodes.begin(), nodes.end(), pn) == nodes.end()) {
    nodes.push_back(pn);
  }
}
void GraphDataEditor::delNode(NODE_PRT pn) {

  if (!pn)
    return;

  for (auto it = pn->child.begin(), itEnd = pn->child.end(); it != itEnd; ++it) {
    auto ch = (*it);
    ch->child.remove(pn);
  }

  if (find(nodes.begin(), nodes.end(), pn) != nodes.end()) {
    nodes.remove(pn);
  }
}


void GraphDataEditor::addEdge(EDGE_PRT pe) {

  if (pe && find(edges.begin(), edges.end(), pe) == edges.end()) {
    edges.push_back(pe);
  }
}

NODE_PRT GraphDataEditor::getHoverNode(double x, double y, double r) {

  for (auto it = nodes.begin(), itEnd = nodes.end(); it != itEnd; ++it) {
    auto n = (*it);
    double dx = x - n->x;
    double dy = y - n->y;
    if (r*r > dx*dx + dy*dy) {
      return n;
    }
  }

  return NULL;
}