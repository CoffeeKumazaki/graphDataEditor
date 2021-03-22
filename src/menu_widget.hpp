#pragma once

enum STATE {
  NOTHING,
  ONLY_NODE,
  ONLY_EDGE,
  NODE_W_EDGE,
  DELETE,
  STATE_NUM
};

std::string state_char[] = {
  "nothing",
  "add node only",
  "connect nodes by edge",
  "add node with edge",
  "delete",
};
