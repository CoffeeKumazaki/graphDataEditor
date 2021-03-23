#include <stdafx.hpp>
#include "struct.hpp"
#include "editor.hpp"

GraphDataEditor::GraphDataEditor() {

}

GraphDataEditor::~GraphDataEditor() {

}

void GraphDataEditor::init() {

  nodes.clear();
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

void GraphDataEditor::output() {

  std::string outfile = "test.txt";
  std::ofstream of(outfile);

  int nCnt = 0;
  int nEdge = 0;
  of << nodes.size() << "\n";
  for (auto it = nodes.begin(), itEnd = nodes.end(); it != itEnd; ++it, nCnt++) {
    auto n = (*it);
    n->id = nCnt;
    of << nCnt << "," << n->x << "," << n->y << "\n";
    nEdge += n->child.size();
  }

  of << nEdge << "\n";
  nCnt = 0;
  for (auto it = nodes.begin(), itEnd = nodes.end(); it != itEnd; ++it) {
    auto n = (*it);
    for (auto cit = n->child.begin(), citEnd = n->child.end(); cit != citEnd; ++cit, nCnt++) {
      auto c = (*cit);
      of << nCnt << "," << n->id << "," << c->id << "\n";
    }
  }
}
