#pragma once

//Only creates positions
//The idea is to put this positions into an addSet of Lines2D or Polygons2D

vector<p2> createRectanglePositions(const p2 corner1, const p2 corner2);

vector<p2> createCirclePositions(const int r, const p2 center, int segments);


//input in radians. It goes from angle1 to angle2, so angle2 is ensured to be ahead. 
// You can specify the number of segments or let it create it based on some arbitrary algorithm
vector<p2> createArc(p2 center, float r, float angle1, float angle2, int segments);

vector<p2> createRoundedSquare(p2 startingPos, float width, float height, float radius);

vector<p2> createRoundedSquare(p2 startingPos, p2 endingPos, float radius);
