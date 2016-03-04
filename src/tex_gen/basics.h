/** \file basics.h
 * Basic types
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

#ifndef NL_BASICS
#define NL_BASICS

#include "nel/misc/types_nl.h"

namespace NLTEXGEN
{

#define NLTEXGEN_SIZE_MIN 1
#define NLTEXGEN_SIZE_MAX 2048

// ***************************************************************************

enum TBlendMode
{
	Replace		= 0,
	Mul,
	Add,
	Sub,
	RSub,
	Screen,
	Darken,
	Lighten,
	Difference,
	Negation,
	Exclusion,
	Overlay,
	HardLight,
	SoftLight,
	ColorDodge,
	RColorDodge,
	ColorBurn,
	RColorBurn,
	Invert,
	
	NumBlendMode
};

// ***************************************************************************

extern const char *BlendModeName[NumBlendMode];

// ***************************************************************************

class CGradientColor
{
public:
	CGradientColor ();

	// The color
	float		Color[4];

	// The color position
	float		Pos;

	// The tangeant position
	float		RightPos;
	bool		RightQuadratic;

	// Operators
	bool operator<(const CGradientColor &other) const
	{
		return Pos < other.Pos;
	}

	bool operator==(const CGradientColor &other) const
	{
		return (Pos == other.Pos) && (RightPos == other.RightPos) && (RightQuadratic && other.RightQuadratic) && 
			(Color[0] == other.Color[0]) && (Color[1] == other.Color[1]) && (Color[2] == other.Color[2]) && 
			(Color[3] == other.Color[3]);
	}

	// Serial this parameter
	void serial(NLMISC::IStream &s);
};

// ***************************************************************************

class CFloatValueSetup
{
public:

	enum TFormula
	{
		Linear,
		TwoPower,
	} Formula;

	CFloatValueSetup () {}
	CFloatValueSetup (TFormula formula, const char *unit, float valueOrigine, float valueTour, float epsilon, bool multitour, bool centered, 
		const float *snapedValues = NULL, uint SnapedValuesCount = 0, float valueLeft=-FLT_MAX, float valueRight=FLT_MAX);

	// Snaped value
	const char	*Unit;
	bool	MultiTour;
	bool	CenterOrigine;
	float	ValueOrigine;
	float	ValueLeft;
	float	ValueRight;
	float	ValueTour;
	float	Epsilon;

	const float		*SnapedValues;
	uint			SnapedValuesCount;
};

// ***************************************************************************

/// A parameter
class CParameter
{
public:
	enum TType
	{
		TypeEnum = 0,			// Blend mode parameter, result in Integer
		TypeColor,				// A simple color, result in Floats
		TypeFloat,				// 0 ~ 1 float edited with a slider, no value feedback, result in Floats[0]
		TypeInt,				// An integer value to set with an edit box and spinners
		TypeGradient,			// A color gradient
		TypeBool,				// A Boolean
		TypeString,				// A String
	}				Type;

	CParameter() {};
		
	// Generic constructor
	CParameter(TType type, const char *name, const char *category, sint firstVersion=0, sint lastVersion=0x7fffffff);

	// Enum constructor
	CParameter(const char *name, const char *category, const char **enumString, int numString, sint firstVersion=0, sint lastVersion=0x7fffffff);

	// Integer constructor
	CParameter(const char *name, const char *category, sint min, sint max, sint firstVersion=0, sint lastVersion=0x7fffffff);

	// Float constructor
	CParameter(const char *name, const char *category, const CFloatValueSetup &setup, sint firstVersion=0, sint lastVersion=0x7fffffff);

	// Bool constructor
	CParameter(const char *name, const char *category, bool value, sint firstVersion=0, sint lastVersion=0x7fffffff);

	// String constructor
	CParameter(const char *name, const char *category, const char *value, bool multiLine, sint firstVersion=0, sint lastVersion=0x7fffffff);

	// Parameter name
	const char	*Name;

	// Parameter category
	const char	*Category;

	// Color
	float		Color[4];

	// Floats
	float		Value;

	// The setup for float value
	CFloatValueSetup	Setup;

	// Gradiant
	std::vector<CGradientColor>	Gradient;

	// Integer
	sint32			Integer;
	sint32			IntegerMin;
	sint32			IntegerMax;

	// Enum strings
	const char		**EnumString;

	// The string
	std::string		StringValue;

	// Version where this parameter exist
	sint			FirstVersion;
	sint			LastVersion;

	// Serial this parameter
	void serial(NLMISC::IStream &s);
};

// ***************************************************************************

/// An input descriptor
class CInput
{
public:
	
	CInput(const char* name)
	{
		Name = name;
	}

	std::string		Name;
};

// ***************************************************************************

enum TChannel
{
	ChannelNone		= 0,
	ChannelRGB		= 1,
	ChannelAlpha	= 2,
	ChannelAll		= -1,
};

// ***************************************************************************

#define NL_DECLARE_COPY_OPERATOR(type,parent_type) \
	parent_type *copy() const \
	{ \
		type *newObject = new type; \
		*newObject = *this; \
		return newObject; \
	}

// ***************************************************************************

void registerTypes();

// ***************************************************************************

const float ColorBlack[4] = {0, 0, 0, 1};
const float ColorRed[4] = {1, 0, 0, 1};
const float ColorGreen[4] = {0, 1, 0, 1};
const float ColorYellow[4] = {1, 1, 0, 1};
const float ColorBlue[4] = {0, 0, 1, 1};
const float ColorMagenta[4] = {1, 0, 1, 1};
const float ColorCyan[4] = {0, 1, 1, 1};
const float ColorWhite[4] = {1, 1, 1, 1};

// ***************************************************************************

class CRenderParameter
{
public:
	CRenderParameter (bool cache, bool draft, bool showHelpers)
	{
		Cache = cache;
		Draft = draft;
		ShowHelpers = showHelpers;
	}
	bool Cache;			// Render the operator in a cash
	bool Draft;			// Render in draft mode
	bool ShowHelpers;	// Show helpers
};

// ***************************************************************************

// Some setup
extern CFloatValueSetup PercentSetup;
extern CFloatValueSetup RandomSetup;
extern CFloatValueSetup CenteredMT1_1Setup;
extern CFloatValueSetup CenteredMT10_10Setup;
extern CFloatValueSetup AngleSetup;
extern CFloatValueSetup ScaleSetup;
extern CFloatValueSetup PixelSetup;
extern sint				FileVersion;

} // NLTEXGEN

#endif // NL_BASICS

/* End of basics.h */
