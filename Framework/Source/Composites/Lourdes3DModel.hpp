#pragma once

#include "Graphics.hpp"


struct Lourdes3DModel
{
	
	Polyhedra casco, patines, vela, orza, soportes, timon;

	

	Lourdes3DModel()
	{
		/*stlToSimplePolyhedra("casco.stl", "casco.bin");
		stlToSimplePolyhedra("patines.stl", "patines.bin");
		stlToSimplePolyhedra("vela.stl", "vela.bin");
		stlToSimplePolyhedra("timon.stl", "timon.bin");
		stlToSimplePolyhedra("orza.stl", "orza.bin");
		stlToSimplePolyhedra("soportes.stl", "soportes.bin");*/

		casco.addPolyhedra("casco.bin");
		patines.addPolyhedra("patines.bin");
		vela.addPolyhedra("vela.bin");
		orza.addPolyhedra("orza.bin");
		soportes.addPolyhedra("soportes.bin");
		timon.addPolyhedra("timon.bin");
	}

};