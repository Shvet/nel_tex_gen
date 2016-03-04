/** \file basics.cpp
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

#include "stdtex_gen.h"
#include "basics.h"
#include "op_fill_color.h"
#include "op_root.h"
#include "op_gradient.h"
#include "op_linear_gradient.h"
#include "op_radial_gradient.h"
#include "op_angular_gradient.h"
#include "op_noise.h"
#include "op_blur.h"
#include "op_mix.h"
#include "op_contrast_luminosity.h"
#include "op_hue_saturation.h"
#include "op_normalize.h"
#include "op_light.h"
#include "op_transform.h"
#include "op_sinus.h"
#include "op_background.h"
#include "op_bars.h"
#include "op_clouds.h"
#include "op_font.h"
#include "op_quad.h"
#include "op_cellular.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

namespace NLTEXGEN
{

// ***************************************************************************

const char *NLTEXGEN::BlendModeName[NumBlendMode]=
{
	"Normal",
	"Multiply",
	"Additive",
	"Substractive",
	"Reverse substractive",
	"Screen",
	"Darken",
	"Lighten",
	"Difference",
	"Negation",
	"Exclusion",
	"Overlay",
	"Hard light",
	"Soft light",
	"Color dodge",
	"Reverse color dodge",
	"Color burn",
	"Reverse color burn",
	"Invert",
};

// ***************************************************************************

CParameter::CParameter(TType type, const char *name, const char *category, sint firstVersion, sint lastVersion)
{
	Name = name;
	Type = type;
	Category = category;
	replaceConstRGBA (Color, ColorBlack, 1);
	EnumString = NULL;
	Integer = 0;
	IntegerMin = 0;
	Value = 0;
	IntegerMax = INT_MAX;
	Setup.Unit = NULL;
	FirstVersion = firstVersion;
	LastVersion = lastVersion;
}

// ***************************************************************************

CParameter::CParameter(const char *name, const char *category, const char **enumString, int numString, sint firstVersion, sint lastVersion)
{
	this->CParameter::CParameter(TypeEnum, name, category, firstVersion, lastVersion);
	EnumString = enumString;
	IntegerMax = numString;
}

// ***************************************************************************

CParameter::CParameter(const char *name, const char *category, sint min, sint max, sint firstVersion, sint lastVersion)
{
	this->CParameter::CParameter(TypeInt, name, category, firstVersion, lastVersion);
	IntegerMin = min;
	IntegerMax = max;
}

// ***************************************************************************

CParameter::CParameter(const char *name, const char *category, const CFloatValueSetup &setup, sint firstVersion, sint lastVersion)
{
	this->CParameter::CParameter(TypeFloat, name, category, firstVersion, lastVersion);
	Setup = setup;
}

// ***************************************************************************

CParameter::CParameter(const char *name, const char *category, bool value, sint firstVersion, sint lastVersion)
{
	this->CParameter::CParameter(TypeBool, name, category, firstVersion, lastVersion);
	Integer = value ? 1 : 0;
}

// ***************************************************************************

CParameter::CParameter(const char *name, const char *category, const char *initValue, bool multiLine, sint firstVersion, sint lastVersion)
{
	this->CParameter::CParameter(TypeString, name, category, firstVersion, lastVersion);
	StringValue = initValue;
	Integer = (sint32)multiLine;
}

// ***************************************************************************

#define	NLMISC_REGISTER_CLASS2(_class_, _name_) NLMISC::CClassRegistry::registerClass(#_name_, _class_::creator, typeid(_class_).name());

void NLTEXGEN::registerTypes()
{
	ITexGenOperator::DefaultBitmap.resize (256, 256);
	float *pixels = ITexGenOperator::DefaultBitmap.getPixels();
	memset (pixels, 0, 256*256*4*sizeof(float));
	NLMISC_REGISTER_CLASS2(COpRoot, Rt);
	NLMISC_REGISTER_CLASS2(COpFillColor, FC);
	NLMISC_REGISTER_CLASS2(COpGradient, Gr);
	NLMISC_REGISTER_CLASS2(COpLinearGradient, LG);
	NLMISC_REGISTER_CLASS2(COpRadialGradient, RG);
	NLMISC_REGISTER_CLASS2(COpAngularGradient, AG);
	NLMISC_REGISTER_CLASS2(COpNoise, Ns);
	NLMISC_REGISTER_CLASS2(COpBlur, Bl);
	NLMISC_REGISTER_CLASS2(COpMix, Mi);
	NLMISC_REGISTER_CLASS2(COpContrastLuminosity, CL);
	NLMISC_REGISTER_CLASS2(COpHueSaturation, HS);
	NLMISC_REGISTER_CLASS2(COpNormalize, No);
	NLMISC_REGISTER_CLASS2(COpLight, Li);
	NLMISC_REGISTER_CLASS2(COpTransform, Tr);
	NLMISC_REGISTER_CLASS2(COpSinus, Si);
	NLMISC_REGISTER_CLASS2(COpBackground, Bg);
	NLMISC_REGISTER_CLASS2(COpBars, Ba);
	NLMISC_REGISTER_CLASS2(COpClouds, Cl);
	NLMISC_REGISTER_CLASS2(COpFont, Ft);
	NLMISC_REGISTER_CLASS2(COpQuad, Qd);
	NLMISC_REGISTER_CLASS2(COpCellular, Cr);
}

// ***************************************************************************

void CParameter::serial(NLMISC::IStream &s)
{
	uint8 value = (uint8)Type;
	s.serial (value);
	Type = (TType)value;

	switch (Type)
	{
	case TypeEnum:
	case TypeBool:
		{
			value = (uint8)Integer;
			s.serial (value);
			Integer = (sint32)value;
		}
		break;
	case TypeInt:
		s.serial (Integer);
		break;
	case TypeColor:
		s.serial (Color[0]);
		s.serial (Color[1]);
		s.serial (Color[2]);
		s.serial (Color[3]);
		break;
	case TypeFloat:
		s.serial (Value);
		break;
	case TypeGradient:
		s.serialCont (Gradient);
		break;
	case TypeString:
		s.serial (StringValue);
		break;
	}
}

// ***************************************************************************

void CGradientColor::serial(NLMISC::IStream &s)
{
	s.serial (Color[0]);
	s.serial (Color[1]);
	s.serial (Color[2]);
	s.serial (Color[3]);
	s.serial (RightPos);
	s.serial (RightQuadratic);

	s.serial (Pos);
}

// ***************************************************************************

CGradientColor::CGradientColor ()
{
	RightPos = 0.5f;
	RightQuadratic = false;
}

// ***************************************************************************

CFloatValueSetup::CFloatValueSetup (TFormula formula, const char *unit, float valueOrigine, float valueTour, float epsilon, bool multitour, bool centered, 
		const float *snapedValues, uint snapedValuesCount, float valueLeft, float valueRight)
{
	Formula = formula;
	Unit = unit;
	ValueOrigine = valueOrigine;
	ValueTour = valueTour;
	Epsilon = epsilon;
	MultiTour = multitour;
	CenterOrigine = centered;
	SnapedValues = snapedValues;
	SnapedValuesCount = snapedValuesCount;
	ValueLeft = valueLeft;
	ValueRight = valueRight;
}

// ***************************************************************************

const float SnapAngles[17]=
{
	0,
	45,
	90,
	135,
	180,
	225,
	270,
	315,
	360,
	-45,
	-90,
	-135,
	-180,
	-225,
	-270,
	-315,
	-360,
};

const float SnapScale[17]=
{
	1,
	2,
	4,
	8,
	16,
	32,
	64,
	127,
	256,
	1.f/2.f,
	1.f/4.f,
	1.f/8.f,
	1.f/16.f,
	1.f/32.f,
	1.f/64.f,
	1.f/128.f,
	1.f/256.f,
};

const float SnapPixel[9]=
{
	0,
	0.25f,
	0.5f,
	0.75f,
	1.f,
	-0.25f,
	-0.5f,
	-0.75f,
	-1.f,
};

const float Snap1[9] = 
{
	0,
	0.5f,
	1.f,
	1.5f,
	2.f,
	-0.5f,
	-1.f,
	-1.5f,
	-2.f,
};

const float Snap10[9] = 
{
	0,
	5,
	10,
	15,
	20,
	-5,
	-10,
	-15,
	-20,
};

CFloatValueSetup PercentSetup (CFloatValueSetup::Linear, "%", 0, 100, 0.001f, false, false, NULL, 0, 0, 100);
CFloatValueSetup RandomSetup (CFloatValueSetup::Linear, "", 0, 1, 0.001f, false, false, NULL, 0, 0, 1);
CFloatValueSetup CenteredMT1_1Setup (CFloatValueSetup::Linear, "", 0, 1, 0.001f, true, true, Snap1, 9);
CFloatValueSetup CenteredMT10_10Setup (CFloatValueSetup::Linear, "", 0, 20, 0.001f, true, true, Snap10, 9);
CFloatValueSetup AngleSetup (CFloatValueSetup::Linear, "°", 0, 360, 0.001f, true, true, SnapAngles, 17);
CFloatValueSetup ScaleSetup (CFloatValueSetup::TwoPower, "times", 0, 2, 0.001f, true, true, SnapScale, 17);
CFloatValueSetup PixelSetup (CFloatValueSetup::Linear, "times", 0, 1, 0.001f, true, true, SnapPixel, 9);

sint				FileVersion;

}

/* End of basics.cpp */
