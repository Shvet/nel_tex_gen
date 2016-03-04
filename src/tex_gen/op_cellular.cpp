/** \file op_cellular.cpp
 * Fill with a color operator
 *
 * $Id: $
 */

/* Copyright, 2003 DIGITAL MURDER.
 *
 * This file is part of DIGITAL MURDER NEL.
 * DIGITAL MURDER NEL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * DIGITAL MURDER NEL is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with DIGITAL MURDER NEL; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#include "stdtex_gen.h"
#include "op_cellular.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

#define NLTEXGEN_CELLULAR_MODE_COUNT 17

const char *ModeName[NLTEXGEN_CELLULAR_MODE_COUNT]=
{
	"cells 0",
	"cells 1",
	"cells 2",
	"cells 3",
	"cells 4",
	"cells 5",
	"cells 6",
	"particles 0",
	"particles 1",
	"particles 2",
	"particles 3",
	"particles 4",
	"particles 5",
	"particles 6",
	"diamond 0",
	"diamond 1",
	"diamond 2",
};

// ***************************************************************************

COpCellular::COpCellular()
{
	// Parameters
	CFloatValueSetup points (CFloatValueSetup::Linear, "points", 2, 100, 1, false, false, NULL, 0, 2);
	_Parameters.push_back (CParameter("points", "cellular", points));
	_Parameters[Points].Value = 20;
	_Parameters.push_back (CParameter("seed", "cellular", RandomSetup));
	_Parameters[Seed].Value = 0;
	_Parameters.push_back (CParameter("mode", "cellular", ModeName, NLTEXGEN_CELLULAR_MODE_COUNT));
}

// ***************************************************************************

inline void addSphere (float &x, float &y, float &r, float x0, float y0, float r0, float x1, float y1, float r1)
{
	float dx = x1 - x0;
	float dy = y1 - y0;
	float p0p1 = (float)sqrt(dx*dx+dy*dy);
	float nx = (x1-x0) / p0p1;
	float ny = (y1-y0) / p0p1;
	float dist = std::max (p0p1+r1, r0);
	float px1 = nx * dist + x0;
	float py1 = ny * dist + y0;
	dist = std::max (p0p1+r0, r1);
	float px0 = x1 - nx * dist;
	float py0 = y1 - ny * dist;
	x = (px0 + px1) * 0.5f;
	y = (py0 + py1) * 0.5f;
	dx = px1 - x;
	dy = py1 - y;
	r = (float)sqrt(dx*dx+dy*dy);
}

// ***************************************************************************

inline bool sphereIntersect (float x0, float y0, float r0, float x1, float y1, float r1)
{
	float dx = (float)fabs(x1 - x0);
	float dy = (float)fabs(y1 - y0);
	if (dx > 0.5f)
		dx = 1.f - dx;
	if (dy > 0.5f)
		dy = 1.f - dy;
	float temp = r1 + r0;
	return (dx*dx+dy*dy) < (temp*temp);
}

// ***************************************************************************

inline float wrap (float value)
{
	if (value < -0.5f)
		return - 1.f - value;
	else if (value > 0.5f)
		return 1.f - value;
	else
		return value;
}

// ***************************************************************************

class CSphereTree
{
public:
	CSphereTree (float x0, float y0, float x1, float y1)
	{
		X0 = x0;
		Y0 = y0;
		X1 = x1;
		Y1 = y1;
		Child0 = NULL;
		Child1 = NULL;
		Radius = 0;
	}

	~CSphereTree()
	{
		if (Child0)
			delete Child0;
		if (Child1)
			delete Child1;
	}

	void addPoint (float x, float y)
	{
		CSphereTree *parent;
		if (findLeave (x, y, parent))
			parent->Child0 = new CSphereTree (parent->X0, parent->Y0, x, y);
		else
			parent->Child1 = new CSphereTree (parent->X1, parent->Y1, x, y);
	}

	void balanceRadius ()
	{
		if (Child0)
			Child0->balanceRadius ();
		
		if (Child1)
			Child1->balanceRadius ();

		if (Child0)
		{
			if (Child1)
			{
				addSphere (Xc, Yc, Radius, Child0->Xc, Child0->Yc, Child0->Radius, 
					Child1->Xc, Child1->Yc, Child1->Radius);
			}
			else
			{
				addSphere (Xc, Yc, Radius, Child0->Xc, Child0->Yc, Child0->Radius, 
					X1, Y1, 0);
			}
		}
		else
		{
			if (Child1)
			{
				addSphere (Xc, Yc, Radius, Child1->Xc, Child1->Yc, Child1->Radius, 
					X0, Y0, 0);
			}
			else
			{
				addSphere (Xc, Yc, Radius, X0, Y0, 0, X1, Y1, 0);
			}
		}
	}

	class CFindStruct
	{
	public:
		CFindStruct (CSphereTree &root, float x, float y)
		{
			X = x;
			Y = y;
			float dx = wrap (X - root.X0);
			float dy = wrap (Y - root.Y0);
			RadiusSq = dx*dx+dy*dy;
			Radius = (float)sqrt(RadiusSq);
			RadiusSq2 = FLT_MAX;
			Radius2 = FLT_MAX;
		}
		float X;
		float Y;
		float Radius;
		float RadiusSq;
		float Radius2;
		float RadiusSq2;
	};

	void findCloser (CFindStruct &find)
	{
		// Intersect ?
		if (sphereIntersect (find.X, find.Y, find.Radius, Xc, Yc, Radius))
		{
			// Check the new point
			float dx = wrap (X1 - find.X);
			float dy = wrap (Y1 - find.Y);
			const float dist = dx*dx+dy*dy;
			if (dist < find.RadiusSq)
			{
				find.RadiusSq = dist;
				find.Radius = (float)sqrt(dist);

				if (Child1)
					Child1->findCloser (find);
				if (Child0)
					Child0->findCloser (find);
			}
			else
			{
				if (Child0)
					Child0->findCloser (find);
				if (Child1)
					Child1->findCloser (find);
			}
		}
	}

	void find2Closers (CFindStruct &find)
	{
		// Intersect ?
		if (sphereIntersect (find.X, find.Y, find.Radius2, Xc, Yc, Radius))
		{
			// Check the new point
			float dx = wrap (X1 - find.X);
			float dy = wrap (Y1 - find.Y);
			const float dist = dx*dx+dy*dy;
			if (dist < find.RadiusSq)
			{
				find.RadiusSq2 = find.RadiusSq;
				find.Radius2 = find.Radius;
				find.RadiusSq = dist;
				find.Radius = (float)sqrt(dist);

				if (Child1)
					Child1->find2Closers (find);
				if (Child0)
					Child0->find2Closers (find);
			}
			else
			{
				if (dist < find.RadiusSq2)
				{
					find.RadiusSq2 = dist;
					find.Radius2 = (float)sqrt(dist);
				}

				if (Child0)
					Child0->find2Closers (find);
				if (Child1)
					Child1->find2Closers (find);
			}
		}
	}

public:
	// Returns true for point 0, false for point 1
	bool findLeave (float x, float y, CSphereTree *&parent)
	{
		CSphereTree *node = this;
		bool left;
		while (node)
		{
			parent = node;
			// todo tile here
			float x0 = node->X0-x;
			float y0 = node->Y0-y;
			float x1 = node->X1-x;
			float y1 = node->Y1-y;
			if ((x0*x0+y0*y0) < (x1*x1+y1*y1))
			{
				node = node->Child0;
				left = true;
			}
			else
			{
				node = node->Child1;
				left = false;
			}
		}
		return left;
	}

	float X0, Y0;
	float X1, Y1;
	float Xc, Yc;
	float Radius;
	CSphereTree *Child0;
	CSphereTree *Child1;
};

TChannel COpCellular::modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	// Blend the input and output
	float *out = output.getPixels();

	// Random
	TRandomValue random = getRandomValue (_Parameters[Seed].Value);
	
	// Build the data structure
	float t0 = getNextRandomValue(random);
	float t1 = getNextRandomValue(random);
	float t2 = getNextRandomValue(random);
	float t3 = getNextRandomValue(random);
	CSphereTree root (t0, t1, t2, t3);

	// Add the points
	uint i;
	const uint count = (uint)_Parameters[Points].Value;
	for (i=2; i<count; i++)
	{
		float x = getNextRandomValue(random);
		float y = getNextRandomValue(random);
		root.addPoint (x, y);
	}

	// Build the cercle
	root.balanceRadius ();

	// For each pixels		
	uint x, y;
	const uint width = output.getWidth();
	const uint height = output.getHeight();
	const float dx = 1.f / (float)width;
	const float dy = 1.f / (float)height;
	float fy = dy/2;
	float minValue = FLT_MAX;
	float maxValue = -FLT_MAX;
	uint mode = _Parameters[Mode].Integer;
	for (y=0, i=0; y<height; y++, fy+=dy)
	{
		float fx = dx/2;
		for (x=0; x<width; x++, fx+=dx, out+=4)
		{
			CSphereTree::CFindStruct find (root, fx, fy);

/*	"cells",
	"round cells",
	"particles",
	"round particles",
	"stained glass",
	"virus",
	*/
			float value;
			switch (mode)
			{
			case 0:
				// Cells 0
				root.findCloser (find);
				value = find.RadiusSq;
				break;
			case 1:
				// Cells 1
				root.findCloser (find);
				value = find.Radius;
				break;
			case 2:
				// Cells 2
				root.find2Closers (find);
				value = (float)sqrt(find.RadiusSq2 + find.RadiusSq);
				break;
			case 3:
				// Cells 3
				root.find2Closers (find);
				value = find.RadiusSq2 + find.RadiusSq;
				break;
			case 4:
				// Cells 4
				root.find2Closers (find);
				value = find.Radius2 + find.Radius;
				break;
			case 5:
				// Cells 5
				root.find2Closers (find);
				value = find.RadiusSq2 * find.RadiusSq;
				break;
			case 6:
				// Cells 6
				root.find2Closers (find);
				value = find.Radius2 * find.Radius;
				break;
			case 7:
				// Particules 0
				root.findCloser (find);
				value = -find.RadiusSq;
				break;
			case 8:
				// Particules 1
				root.findCloser (find);
				value = -find.Radius;
				break;
			case 9:
				// Particules 2
				root.find2Closers (find);
				value = 1.f-(float)sqrt(find.RadiusSq2 + find.RadiusSq);
				break;
			case 10:
				// Particules 3
				root.find2Closers (find);
				value = 1.f-(find.RadiusSq2 + find.RadiusSq);
				break;
			case 11:
				// Particules 4
				root.find2Closers (find);
				value = 1.f-(find.Radius2 + find.Radius);
				break;
			case 12:
				// Particules 5
				root.find2Closers (find);
				value = 1.f - find.RadiusSq2 * find.RadiusSq;
				break;
			case 13:
				// Particules 6
				root.find2Closers (find);
				value = 1.f - find.Radius2 * find.Radius;
				break;
			case 14:
				// Diamonds 0
				root.find2Closers (find);
				value = find.RadiusSq2 - find.RadiusSq;
				break;
			case 15:
				// Diamonds 1
				root.find2Closers (find);
				value = (float)sqrt(find.RadiusSq2 - find.RadiusSq);
				break;
			case 16:
				// Diamonds 2
				root.find2Closers (find);
				value = find.Radius2 - find.Radius;
				break;
			}

			if (value < minValue)
				minValue = value;
			if (value > maxValue)
				maxValue = value;

			out[0] = value;
		}
	}
	const uint count2 = output.size();
	out = output.getPixels();
	const float delta = 1.f / (maxValue - minValue);
	for (i=0; i<count2; i++, out+=4)
	{
		out[0] = (out[0]-minValue) * delta;
		out[1] = out[0];
		out[2] = out[0];
	}

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_cellular.cpp */
