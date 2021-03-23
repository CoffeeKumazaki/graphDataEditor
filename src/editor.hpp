#pragma once

struct Node;
struct Edge;

class GraphDataEditor
{

public:
  GraphDataEditor();
  ~GraphDataEditor();

  void init();
  void addNode(NODE_PRT pn);
  void delNode(NODE_PRT pn);
  int numNode() { return nodes.size(); }

  ND_CIT nodeBegin() { return nodes.begin(); }
  ND_CIT nodeEnd() { return nodes.end(); }

  NODE_PRT getHoverNode(double x, double y, double r);

  void output();

private:
  std::list<NODE_PRT> nodes;
};