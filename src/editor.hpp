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
  void addEdge(EDGE_PRT pe);
  int numNode() { return nodes.size(); }
  int numEdge() { return edges.size(); }

  ND_CIT nodeBegin() { return nodes.begin(); }
  ND_CIT nodeEnd() { return nodes.end(); }
  EG_CIT edgeBegin() { return edges.begin(); }
  EG_CIT edgeEnd() { return edges.end(); }

  NODE_PRT getHoverNode(double x, double y, double r);

private:
  std::list<NODE_PRT> nodes;
  std::list<EDGE_PRT> edges;
};