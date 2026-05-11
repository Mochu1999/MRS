#pragma once
struct IconLight 
{
	Polyhedra interm;
	Polygons2D mainHull, sideHull1, sideHull2, aux1, aux2, aux3;
	Lines2D auxa, auxb;

	IconLight()
	{

		/*stlToBinary(interm, "mainHull.stl");
		polyhedraTo2D(interm, mainHull);

		stlToBinary(interm, "sideHull1.stl");
		polyhedraTo2D(interm, sideHull1);

		stlToBinary(interm, "sideHull2.stl");
		polyhedraTo2D(interm, sideHull2);

		stlToBinary(interm, "aux1.stl");
		polyhedraTo2D(interm, aux1);

		stlToBinary(interm, "aux2.stl");
		polyhedraTo2D(interm, aux2);

		stlToBinary(interm, "aux3.stl");
		polyhedraTo2D(interm, aux3);*/


		auxa.addSet({ {-5,0},{5,0} });
		auxb.addSet({ {0,-5},{0,5} });

	}

	

};






struct ArrowsMeteo
{
	Lines2DInstanced lines;
	
	//Normalized arrow
	vector<p2> positions = { {0,0},{0,-0.019},{0.88,-0.019},{0.714,-0.145},{0.742,-0.175},{1,0}
		, { 0.742,0.175 },{0.714,0.145},{0.88,0.019},{0,0.019},{0,0} };

	vector<unsigned int> indices = { 0,1,2 ,2,3,4 ,2,4,5, 0,2,5 ,5,6,7 ,5,7,8 ,5,8,0, 8,9,0 };

	ArrowsMeteo()
	{

		lines.addInitialSet(positions);
		lines.addInstances({ {  { 50,50 },0,{100,100} } });

	}

	void update(const vector<InstanceAttributes>& currentInstancing)
	{
		lines.clear();
		lines.addInitialSet(positions);
		lines.addInstances(currentInstancing);
	}

	
};