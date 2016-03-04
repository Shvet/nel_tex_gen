/** \file transform_float.h
 * Blend operations
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

#ifndef NL_TRANSFORM_BLEND
#define NL_TRANSFORM_BLEND

#include "basics.h"

namespace NLTEXGEN
{

#define NLTEXGEN_TRANSFORM_MACRO \
	static inline void transform (float *dst, const float *f0, const float *f1, uint size) \
	{ \
		uint i; \
		for (i=0; i<size; i++) \
			dst[i] = transform (f0[i], f1[i]); \
	} \

class CCopy
{
public:
	static inline float transform (float f0, float f1)
	{
		return f1;
	}
	static inline void transform (float *dst, const float *f0, const float *f1, uint size)
	{
		memcpy (dst, f1, size*sizeof(float));
	}
};

class CInvert
{
public:
	static inline float transform (float f0, float f1)
	{
		return 1.f-f0;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CMul
{
public:
	static inline float transform (float f0, float f1)
	{
		return f0 * f1;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CAdd
{
public:
	static inline float transform (float f0, float f1)
	{
		return f0 + f1;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CSub
{
public:
	static inline float transform (float f0, float f1)
	{
		return f0 - f1;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CRSub
{
public:
	static inline float transform (float f0, float f1)
	{
		return f1 - f0;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CScreen
{
public:
	static inline float transform (float f0, float f1)
	{
		return 1.f - (1.f - f0) * (1.f - f1);
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CDarken
{
public:
	static inline float transform (float f0, float f1)
	{
		return (f0<f1)?f0:f1;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CLighten
{
public:
	static inline float transform (float f0, float f1)
	{
		return (f0>f1)?f0:f1;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CDifference
{
public:
	static inline float transform (float f0, float f1)
	{
		return (float)fabs(f0-f1);
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CNegation
{
public:
	static inline float transform (float f0, float f1)
	{
		return 1.f - (float)(1.f - f0 - f1);
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CExclusion
{
public:
	static inline float transform (float f0, float f1)
	{
		return f0 + f1 - 2.f * f0 * f1;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class COverlay
{
public:
	static inline float transform (float f0, float f1)
	{
		return (f1<0.5f)?2.f*f0*f1:1.f-2.f*(1-f0)*(1-f1);
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CHardLight
{
public:
	static inline float transform (float f0, float f1)
	{
		return (f0<0.5f)?2.f*f0*f1:1.f-2.f*(1-f0)*(1-f1);
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CSoftLight
{
public:
	static inline float transform (float f0, float f1)
	{
	  if (f0<0.5f)
		  return f1-(0.5f-f0)*(0.5f-(0.5f-f1)*(0.5f-f1));
	  else
		return f1+(f0-0.5f)*(0.5f-(0.5f-f1)*(0.5f-f1));
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CColorDodge
{
public:
	static inline float transform (float f0, float f1)
	{
		float div = (1.f == f0)?1.f/256.f:1.f-f0;
		float tt = f1 / div;
		return tt;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CRColorDodge
{
public:
	static inline float transform (float f0, float f1)
	{
		float div = (1.f == f1)?1.f/256.f:1.f-f1;
		float tt = f0 / div;
		return tt;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CColorBurn
{
public:
	static inline float transform (float f0, float f1)
	{
		float div = (0.f == f0)?1.f/256.f:f0;
		return 1.f - (1.f - f1) / div;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

class CRColorBurn
{
public:
	static inline float transform (float f0, float f1)
	{
		float div = (0.f == f1)?1.f/256.f:f1;
		return 1.f - (1.f - f0) / div;
	}
	NLTEXGEN_TRANSFORM_MACRO
};

inline void copy(float *dst, const float *src0);
inline void add(float *dst, const float *src0);
inline void sub(float *dst, const float *src0);
inline void rsub(float *dst, const float *src0);
inline void mul(float *dst, const float *src0);
inline void screen(float *dst, const float *src0);
inline void mul(float *dst, const float factor);
inline void div(float *dst, const float factor);

inline void replaceRGBA (float *dst, const float *src0, const uint size);
inline void replaceRGB (float *dst, const float *src0, const uint size);
inline void replaceA (float *dst, const float *src0, const uint size);
inline void replaceBlendRGBA (float *dst, const float *src0, const uint size, const float alpha);
inline void replaceBlendRGB (float *dst, const float *src0, const uint size, const float alpha);
inline void replaceBlendA (float *dst, const float *src0, const uint size, const float alpha);
inline void replaceBlendMaskRGBA (float *dst, const float *src0, const uint size, const float alpha);
inline void replaceBlendMaskRGB (float *dst, const float *src0, const uint size, const float alpha);
inline void replaceBlendMaskA (float *dst, const float *src0, const uint size, const float alpha);
inline void replaceConstRGBA (float *dst, const float *color, const uint size);
inline void replaceConstRGB (float *dst, const float *color, const uint size);
inline void replaceConstA (float *dst, const float alpha, const uint size);
inline void replaceBlendConstRGBA (float *dst, const float *color, const uint size, const float alpha);
inline void replaceBlendConstRGB (float *dst, const float *color, const uint size, const float alpha);
inline void replaceBlendConstA (float *dst, const float alphaColor, const uint size, const float alpha);
// For i in [0~size] : dst[i].channel = src0[i].channel
inline void replace (float *dst, const float *src0, const uint size, const TChannel channel);
// For i in [0~size] : dst[i].channel = src0[i].channel * alpha + dst[i].channel * (1-alpha)
inline void replaceBlend (float *dst, const float *src0, const uint size, const TChannel channel, const float alpha);
inline void replaceBlendMask (float *dst, const float *src0, const float *mask, const uint size, const TChannel channel, const float alpha);
// For i in [0~size] : dst[i].channel = color.channel
inline void replaceConst (float *dst, const float *color, const uint size, const TChannel channel);
// For i in [0~size] : dst[i].channel = color.channel * alpha + dst[i].channel * (1 - alpha)
inline void replaceBlendConst (float *dst, const float *color, const uint size, const TChannel channel, const float alpha);
// For i in [0~size] : dst[i].channel = blendMode(src0[i].channel, src1[i].channel)
inline void transform (float *dst, const float *src0, const float *src1, uint size, TBlendMode mode, TChannel channel);
// For i in [0~size] : dst[i].channel = blendMode(src0[i].channel, src0[i].channel) * (alpha*mask[i]) + dst[i].channel * (1-(alpha*mask[i]))
inline void transformBlend (float *dst, const float *src0, const float *src1, const float *mask, uint size, TBlendMode mode, TChannel channel, float alpha);
// For i in [0~size] : dst[i].channel = blendMode(src[i].channel, color.channel)
inline void transformConst (float *dst, const float *src, const float *color, uint size, TBlendMode mode, TChannel channel);
// For i in [0~size] : dst[i].channel = blendMode(src[i].channel, color.channel) * alpha + dst[i].channel * (1-alpha)
inline void transformBlendConst (float *dst, const float *src, const float *color, uint size, TBlendMode mode, TChannel channel, float alpha);

// Get the gradient value
inline void gradient (float *dst, const std::vector<CGradientColor> &gradient, float offset);

// Gradient a gray scale bitmap (use the R component)
void gradient (float *dst, float *gradientPixels, uint width, uint height, const std::vector<CGradientColor> &_gradient, bool filter, bool tile, bool mirror, bool invert, bool wrapFilter);

// Init the gaussian
inline void initGaussian (float radius, float *values, uint size);

// Bilinear
inline void bilinearRGB (float *dst, const float *p00, const float *p01, const float *p10, const float *p11, 
						 const float x, const float y);
inline void bilinearSampleTileRGB (float *dst, const float *src, float x, float y, uint widthMask, 
						   uint heightMask, uint widthShift, bool tileX, bool tileY);

template<class A>
inline void copyPixels (float *dst, uint dstWidth, uint dstHeight, sint dstX, sint dstY, const float *src, uint srcWidth, 
				 uint srcHeight, bool tileX, bool tileY, const A &transfo);

template<class A>
inline void addPixels (float *dst, uint dstWidth, uint dstHeight, sint dstX, sint dstY, const float *src, uint srcWidth, 
				 uint srcHeight, bool tileX, bool tileY, float *colorToTransform, const A &transfo);

void HVS2RGB (float h, float v, float s, float *color);
void RGB2HVS (const float *color, float &h, float &v, float &s);
float easineasout(float x);

// Draw a constant color quad
void drawQuad (float *dst, sint x, sint y, uint width, uint height, uint quadwidth, uint quadheight, const float *color, bool tileU, bool tileV);

// ***************************************************************************
// Random
// ***************************************************************************

typedef uint32 TRandomValue;
inline TRandomValue getRandomValue (float value);
inline float getNextRandomValue (TRandomValue &previous);

// ***************************************************************************
// Simple RGBA operators
// ***************************************************************************

inline void copy(float *dst, const float *src)
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
	dst[3] = src[3];
}

// ***************************************************************************

inline void add(float *dst, const float *src)
{
	dst[0] += src[0];
	dst[1] += src[1];
	dst[2] += src[2];
	dst[3] += src[3];
}

// ***************************************************************************

inline void sub(float *dst, const float *src)
{
	dst[0] -= src[0];
	dst[1] -= src[1];
	dst[2] -= src[2];
	dst[3] -= src[3];
}

// ***************************************************************************

inline void rsub(float *dst, const float *src)
{
	dst[0] = src[0] - dst[0];
	dst[1] = src[1] - dst[1];
	dst[2] = src[2] - dst[2];
	dst[3] = src[3] - dst[3];
}

// ***************************************************************************

inline void mul(float *dst, const float *src)
{
	dst[0] *= src[0];
	dst[1] *= src[1];
	dst[2] *= src[2];
	dst[3] *= src[3];
}

// ***************************************************************************

inline void mul(float *dst, const float factor)
{
	dst[0] *= factor;
	dst[1] *= factor;
	dst[2] *= factor;
	dst[3] *= factor;
}

// ***************************************************************************

inline void screen(float *dst, const float *src)
{
	uint i;
	for (i=0; i<4; i++)
		dst[i] = 1.f - (1.f - dst[i]) * (1.f - src[i]);
}

// ***************************************************************************

inline void div(float *dst, const float factor)
{
	dst[0] /= factor;
	dst[1] /= factor;
	dst[2] /= factor;
	dst[3] /= factor;
}

// ***************************************************************************
// RGBA manipulators
// ***************************************************************************

inline void replaceRGBA (float *dst, const float *src0, const uint size)
{
	memcpy (dst, src0, size*sizeof(float));
}

// ***************************************************************************

template<class A>
inline void templateTransfoRGBA (float *dst, const float *src0, const float *src1, const uint size, const A &transfo)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i+0] = transfo.transform(src0[i+0], src1[i+0]);
		dst[i+1] = transfo.transform(src0[i+1], src1[i+1]);
		dst[i+2] = transfo.transform(src0[i+2], src1[i+2]);
		dst[i+3] = transfo.transform(src0[i+3], src1[i+3]);
	}
}

// ***************************************************************************
// RGB manipulators
// ***************************************************************************

inline void replaceRGB (float *dst, const float *src0, const uint size)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = src0[i];
		dst[i+1] = src0[i+1];
		dst[i+2] = src0[i+2];
	}
}

// ***************************************************************************

template<class A>
inline void templateTransfoRGB (float *dst, const float *src0, const float *src1, const uint size, const A &transfo)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i+0] = transfo.transform(src0[i+0], src1[i+0]);
		dst[i+1] = transfo.transform(src0[i+1], src1[i+1]);
		dst[i+2] = transfo.transform(src0[i+2], src1[i+2]);
	}
}

// ***************************************************************************
// Alpha manipulators
// ***************************************************************************

inline void replaceA (float *dst, const float *src0, const uint size)
{
	uint i;
	for (i=3; i<size; i+=4)
	{
		dst[i] = src0[i];
	}
}

// ***************************************************************************

template<class A>
inline void templateTransfoA (float *dst, const float *src0, const float *src1, const uint size, const A &transfo)
{
	uint i;
	for (i=3; i<size; i+=4)
	{
		dst[i] = transfo.transform(src0[i], src1[i]);
	}
}

// ***************************************************************************
// RGBA blend manipulators
// ***************************************************************************

inline void replaceBlendRGBA (float *dst, const float *src0, const uint size, const float alpha)
{
	const float alphaMinus = 1.f - alpha;
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = src0[i]*alpha + dst[i]*alphaMinus;
		dst[i+1] = src0[i+1]*alpha + dst[i+1]*alphaMinus;
		dst[i+2] = src0[i+2]*alpha + dst[i+2]*alphaMinus;
		dst[i+3] = src0[i+3]*alpha + dst[i+3]*alphaMinus;
	}
}

// ***************************************************************************

inline void replaceBlendMaskRGBA (float *dst, const float *src0, const float *mask, const uint size, const float alpha)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		const float pixelAlpha = mask[i] * alpha;
		const float pixelAlphaMinus = 1.f - pixelAlpha;
		dst[i] = src0[i]*pixelAlpha + dst[i]*pixelAlphaMinus;
		dst[i+1] = src0[i+1]*pixelAlpha + dst[i+1]*pixelAlphaMinus;
		dst[i+2] = src0[i+2]*pixelAlpha + dst[i+2]*pixelAlphaMinus;
		dst[i+3] = src0[i+3]*pixelAlpha + dst[i+3]*pixelAlphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendRGBA (float *dst, const float *src0, const float *src1, const uint size, const float alpha, const A &transfo)
{
	const float alphaMinus = 1.f - alpha;
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = (transfo.transform(src0[i+0], src1[i+0]))*alpha + dst[i+0]*alphaMinus;
		dst[i+1] = (transfo.transform(src0[i+1], src1[i+1]))*alpha + dst[i+1]*alphaMinus;
		dst[i+2] = (transfo.transform(src0[i+2], src1[i+2]))*alpha + dst[i+2]*alphaMinus;
		dst[i+3] = (transfo.transform(src0[i+3], src1[i+3]))*alpha + dst[i+3]*alphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendMaskRGBA (float *dst, const float *src0, const float *src1, const float *mask, const uint size, const float alpha, const A &transfo)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		const float pixelAlpha = mask[i] * alpha;
		const float pixelAlphaMinus = 1.f - pixelAlpha;
		dst[i] = (transfo.transform(src0[i+0], src1[i+0]))*pixelAlpha + dst[i+0]*pixelAlphaMinus;
		dst[i+1] = (transfo.transform(src0[i+1], src1[i+1]))*pixelAlpha + dst[i+1]*pixelAlphaMinus;
		dst[i+2] = (transfo.transform(src0[i+2], src1[i+2]))*pixelAlpha + dst[i+2]*pixelAlphaMinus;
		dst[i+3] = (transfo.transform(src0[i+3], src1[i+3]))*pixelAlpha + dst[i+3]*pixelAlphaMinus;
	}
}

// ***************************************************************************
// RGB blend manipulators
// ***************************************************************************

inline void replaceBlendRGB (float *dst, const float *src0, const uint size, const float alpha)
{
	const float alphaMinus = 1.f - alpha;
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = src0[i]*alpha + dst[i]*alphaMinus;
		dst[i+1] = src0[i+1]*alpha + dst[i+1]*alphaMinus;
		dst[i+2] = src0[i+2]*alpha + dst[i+2]*alphaMinus;
	}
}

// ***************************************************************************

inline void replaceBlendMaskRGB (float *dst, const float *src0, const float *mask, const uint size, const float alpha)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		const float pixelAlpha = mask[i] * alpha;
		const float pixelAlphaMinus = 1.f - pixelAlpha;
		dst[i] = src0[i]*pixelAlpha + dst[i]*pixelAlphaMinus;
		dst[i+1] = src0[i+1]*pixelAlpha + dst[i+1]*pixelAlphaMinus;
		dst[i+2] = src0[i+2]*pixelAlpha + dst[i+2]*pixelAlphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendRGB (float *dst, const float *src0, const float *src1, const uint size, const float alpha, const A &transfo)
{
	const float alphaMinus = 1.f - alpha;
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = (transfo.transform(src0[i], src1[i]))*alpha + dst[i]*alphaMinus;
		dst[i+1] = (transfo.transform(src0[i+1], src1[i+1]))*alpha + dst[i+1]*alphaMinus;
		dst[i+2] = (transfo.transform(src0[i+2], src1[i+2]))*alpha + dst[i+2]*alphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendMaskRGB (float *dst, const float *src0, const float *src1, const float *mask, const uint size, const float alpha, const A &transfo)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		const float pixelAlpha = mask[i] * alpha;
		const float pixelAlphaMinus = 1.f - pixelAlpha;
		dst[i] = (transfo.transform(src0[i], src1[i]))*pixelAlpha + dst[i]*pixelAlphaMinus;
		dst[i+1] = (transfo.transform(src0[i+1], src1[i+1]))*pixelAlpha + dst[i+1]*pixelAlphaMinus;
		dst[i+2] = (transfo.transform(src0[i+2], src1[i+2]))*pixelAlpha + dst[i+2]*pixelAlphaMinus;
	}
}

// ***************************************************************************
// RGB Src0Alpha blend manipulators
// ***************************************************************************

inline void replaceBlendRGBSrc0Alpha (float *dst, const float *src0, const uint size, const float alpha)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		const float a = alpha * src0[i+3];
		const float alphaMinus = 1.f - a;
		dst[i] = src0[i]*a + dst[i]*alphaMinus;
		dst[i+1] = src0[i+1]*a + dst[i+1]*alphaMinus;
		dst[i+2] = src0[i+2]*a + dst[i+2]*alphaMinus;
	}
}

// ***************************************************************************

inline void replaceBlendMaskRGBSrc0Alpha (float *dst, const float *src0, const float *mask, const uint size, const float alpha)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		const float pixelAlpha = mask[i] * alpha * src0[i+3];
		const float pixelAlphaMinus = 1.f - pixelAlpha;
		dst[i] = src0[i]*pixelAlpha + dst[i]*pixelAlphaMinus;
		dst[i+1] = src0[i+1]*pixelAlpha + dst[i+1]*pixelAlphaMinus;
		dst[i+2] = src0[i+2]*pixelAlpha + dst[i+2]*pixelAlphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendRGBSrc0Alpha (float *dst, const float *src0, const float *src1, const uint size, const float alpha, const A &transfo)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		const float pixelAlpha = alpha * src0[i+3];
		const float alphaMinus = 1.f - pixelAlpha;
		dst[i] = (transfo.transform(src0[i], src1[i]))*pixelAlpha + dst[i]*alphaMinus;
		dst[i+1] = (transfo.transform(src0[i+1], src1[i+1]))*pixelAlpha + dst[i+1]*alphaMinus;
		dst[i+2] = (transfo.transform(src0[i+2], src1[i+2]))*pixelAlpha + dst[i+2]*alphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendMaskRGBSrc0Alpha (float *dst, const float *src0, const float *src1, const float *mask, const uint size, const float alpha, const A &transfo)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		const float pixelAlpha = mask[i] * alpha * src0[i+3];
		const float pixelAlphaMinus = 1.f - pixelAlpha;
		dst[i] = (transfo.transform(src0[i], src1[i]))*pixelAlpha + dst[i]*pixelAlphaMinus;
		dst[i+1] = (transfo.transform(src0[i+1], src1[i+1]))*pixelAlpha + dst[i+1]*pixelAlphaMinus;
		dst[i+2] = (transfo.transform(src0[i+2], src1[i+2]))*pixelAlpha + dst[i+2]*pixelAlphaMinus;
	}
}

// ***************************************************************************
// Alpha blend manipulators
// ***************************************************************************

inline void replaceBlendA (float *dst, const float *src0, const uint size, const float alpha)
{
	const float alphaMinus = 1.f - alpha;
	uint i;
	for (i=3; i<size; i+=4)
	{
		dst[i] = src0[i]*alpha + dst[i]*alphaMinus;
	}
}

// ***************************************************************************

inline void replaceBlendMaskA (float *dst, const float *src0, const float *mask, const uint size, const float alpha)
{
	uint i;
	for (i=3; i<size; i+=4)
	{
		const float pixelAlpha = mask[i] * alpha;
		const float pixelAlphaMinus = 1.f - pixelAlpha;
		dst[i] = src0[i]*pixelAlpha + dst[i]*pixelAlphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendA (float *dst, const float *src0, const float *src1, const uint size, const float alpha, const A &transfo)
{
	const float alphaMinus = 1.f - alpha;
	uint i;
	for (i=3; i<size; i+=4)
	{
		dst[i] = (transfo.transform(src0[i], src1[i]))*alpha + dst[i]*alphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendMaskA (float *dst, const float *src0, const float *src1, const float *mask, const uint size, const float alpha, const A &transfo)
{
	uint i;
	for (i=3; i<size; i+=4)
	{
		const float pixelAlpha = mask[i] * alpha;
		const float pixelAlphaMinus = 1.f - pixelAlpha;
		dst[i] = (transfo.transform(src0[i], src1[i]))*pixelAlpha + dst[i]*pixelAlphaMinus;
	}
}

// ***************************************************************************
// RGBA const manipulators
// ***************************************************************************

inline void replaceConstRGBA (float *dst, const float *color, const uint size)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = color[0];
		dst[i+1] = color[1];
		dst[i+2] = color[2];
		dst[i+3] = color[3];
	}
}

// ***************************************************************************

template<class A>
inline void templateConstRGBA (float *dst, const float *src, const float *color, const uint size, const A &transfo)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = transfo.transform(color[0], src[i]);
		dst[i+1] = transfo.transform(color[1], src[i+1]);
		dst[i+2] = transfo.transform(color[2], src[i+2]);
		dst[i+3] = transfo.transform(color[3], src[i+3]);
	}
}

// ***************************************************************************

template<class A>
inline void templateConstAddRGBA (float *dst, const float *src, const float *color, const uint size, const A &transfo)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] += transfo.transform(color[0], src[i]);
		dst[i+1] += transfo.transform(color[1], src[i+1]);
		dst[i+2] += transfo.transform(color[2], src[i+2]);
		dst[i+3] += transfo.transform(color[3], src[i+3]);
	}
}

// ***************************************************************************
// RGB const manipulators
// ***************************************************************************

inline void replaceConstRGB (float *dst, const float *color, const uint size)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = color[i];
		dst[i+1] = color[i+1];
		dst[i+2] = color[i+2];
	}
}

// ***************************************************************************

template<class A>
inline void templateConstRGB (float *dst, const float *src, const float *color, const uint size, const A &transfo)
{
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = transfo.transform(src[i], color[0]);
		dst[i+1] = transfo.transform(src[i+1], color[1]);
		dst[i+2] = transfo.transform(src[i+2], color[2]);
	}
}

// ***************************************************************************
// Alpha const manipulators
// ***************************************************************************

inline void replaceConstA (float *dst, const float alpha, const uint size)
{
	uint i;
	for (i=3; i<size; i+=4)
	{
		dst[i] = alpha;
	}
}

// ***************************************************************************

template<class A>
inline void templateConstA (float *dst, const float *src, const float alpha, const uint size, const A &transfo)
{
	uint i;
	for (i=3; i<size; i+=4)
	{
		dst[i] = transfo.transform(src[i], alpha);
	}
}

// ***************************************************************************
// RGBA blend const manipulators
// ***************************************************************************

inline void replaceBlendConstRGBA (float *dst, const float *color, const uint size, const float alpha)
{
	const float alphaMinus = 1.f - alpha;
	const float alphaColor[4] = {color[0]*alpha, color[1]*alpha, color[2]*alpha, color[3]*alpha};
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = alphaColor[0] + dst[i]*alphaMinus;
		dst[i+1] = alphaColor[1] + dst[i+1]*alphaMinus;
		dst[i+2] = alphaColor[2] + dst[i+2]*alphaMinus;
		dst[i+3] = alphaColor[3] + dst[i+3]*alphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendConstRGBA (float *dst, const float *src, const float *color, const uint size, const float alpha, const A &transfo)
{
	const float alphaMinus = 1.f - alpha;
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = transfo.transform(color[0], src[i+0])*alpha + dst[i]*alphaMinus;
		dst[i+1] = transfo.transform(color[1], src[i+1])*alpha + dst[i+1]*alphaMinus;
		dst[i+2] = transfo.transform(color[2], src[i+2])*alpha + dst[i+2]*alphaMinus;
		dst[i+3] = transfo.transform(color[3], src[i+3])*alpha + dst[i+3]*alphaMinus;
	}
}

// ***************************************************************************
// RGB blend const manipulators
// ***************************************************************************

inline void replaceBlendConstRGB (float *dst, const float *color, const uint size, const float alpha)
{
	const float alphaMinus = 1.f - alpha;
	const float alphaColor[3] = {color[0]*alpha, color[1]*alpha, color[2]*alpha};
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = alphaColor[0] + dst[i]*alphaMinus;
		dst[i+1] = alphaColor[1] + dst[i+1]*alphaMinus;
		dst[i+2] = alphaColor[2] + dst[i+2]*alphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendConstRGB (float *dst, const float *src, const float *color, const uint size, const float alpha, const A &transfo)
{
	const float alphaMinus = 1.f - alpha;
	uint i;
	for (i=0; i<size; i+=4)
	{
		dst[i] = transfo.transform(color[0], src[i])*alpha + dst[i]*alphaMinus;
		dst[i+1] = transfo.transform(color[1], src[i+1])*alpha + dst[i]*alphaMinus;
		dst[i+2] = transfo.transform(color[2], src[i+2])*alpha + dst[i]*alphaMinus;
	}
}

// ***************************************************************************
// Alpha blend const manipulators
// ***************************************************************************

inline void replaceBlendConstA (float *dst, const float alphaColor, const uint size, const float alpha)
{
	const float alphaMinus = 1.f - alpha;
	const float alphaColorAlpha = alpha*alphaColor;
	uint i;
	for (i=3; i<size; i+=4)
	{
		dst[i] = alphaColorAlpha + dst[i]*alphaMinus;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendConstA (float *dst, const float *src, const float alphaColor, const uint size, const float alpha, const A &transfo)
{
	const float alphaMinus = 1.f - alpha;
	uint i;
	for (i=3; i<size; i+=4)
	{
		dst[i] = transfo.transform (alphaColor, src[i]) * alpha + dst[i]*alphaMinus;
	}
}

// ***************************************************************************
// Manipulators
// ***************************************************************************

// For i in [0~size] : dst[i].channel = src0[i].channel
inline void replace (float *dst, const float *src0, const uint size, const TChannel channel)
{
	switch (channel)
	{
	case ChannelAll:
		replaceRGBA(dst, src0, size);
		return;
	case ChannelRGB:
		replaceRGB(dst, src0, size);
		return;
	case ChannelAlpha:
		replaceA(dst, src0, size);
		return;
	}
}

// ***************************************************************************

template<class A>
inline void templateTransfo (float *dst, const float *src0, const float *src1, const uint size, const TChannel channel, const A &transfo)
{
	switch (channel)
	{
	case ChannelAll:
		templateTransfoRGBA (dst, src0, src1, size, transfo);
		return;
	case ChannelRGB:
		templateTransfoRGB (dst, src0, src1, size, transfo);
		return;
	case ChannelAlpha:
		templateTransfoA (dst, src0, src1, size, transfo);
		return;
	default:
		break;
	}
}

// ***************************************************************************
// Blend manipulators
// ***************************************************************************

// For i in [0~size] : dst[i].channel = src0[i].channel * alpha + dst[i].channel * (1-alpha)
inline void replaceBlend (float *dst, const float *src0, const uint size, const TChannel channel, const float alpha)
{
	switch (channel)
	{
	case ChannelAll:
		replaceBlendRGBA(dst, src0, size, alpha);
		return;
	case ChannelRGB:
		replaceBlendRGB(dst, src0, size, alpha);
		return;
	case ChannelAlpha:
		replaceBlendA(dst, src0, size, alpha);
		return;
	}
}

// ***************************************************************************

// For i in [0~size] : dst[i].channel = src0[i].channel * (alpha*mask[i]) + dst[i].channel * (1-(alpha*mask[i]))
inline void replaceBlendMask (float *dst, const float *src0, const float *mask, const uint size, const TChannel channel, const float alpha)
{
	switch (channel)
	{
	case ChannelAll:
		replaceBlendMaskRGBA(dst, src0, mask, size, alpha);
		return;
	case ChannelRGB:
		replaceBlendMaskRGB(dst, src0, mask, size, alpha);
		return;
	case ChannelAlpha:
		replaceBlendMaskA(dst, src0, mask, size, alpha);
		return;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlend (float *dst, const float *src0, const float *src1, const uint size, const TChannel channel, const float alpha, const A &transfo)
{
	switch (channel)
	{
	case ChannelAll:
		templateBlendRGBA<A> (dst, src0, src1, size, alpha, transfo);
		return;
	case ChannelRGB:
		templateBlendRGB (dst, src0, src1, size, alpha, transfo);
		return;
	case ChannelAlpha:
		templateBlendA<A> (dst, src0, src1, size, alpha, transfo);
		return;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendMask (float *dst, const float *src0, const float *src1, const float *mask, const uint size, const TChannel channel, const float alpha, const A &transfo)
{
	switch (channel)
	{
	case ChannelAll:
		templateBlendMaskRGBA<A> (dst, src0, src1, mask, size, alpha, transfo);
		return;
	case ChannelRGB:
		templateBlendMaskRGB<A> (dst, src0, src1, mask, size, alpha, transfo);
		return;
	case ChannelAlpha:
		templateBlendMaskA<A> (dst, src0, src1, mask, size, alpha, transfo);
		return;
	}
}

// ***************************************************************************
// Blend Src0 Alpha manipulators
// ***************************************************************************

// For i in [0~size] : dst[i].channel = src0[i].channel * alpha + dst[i].channel * (1-alpha)
inline void replaceBlendSrc0Alpha (float *dst, const float *src0, const uint size, const float alpha)
{
	replaceBlendRGBSrc0Alpha (dst, src0, size, alpha);
}

// ***************************************************************************

// For i in [0~size] : dst[i].channel = src0[i].channel * (alpha*mask[i]) + dst[i].channel * (1-(alpha*mask[i]))
inline void replaceBlendMaskSrc0Alpha (float *dst, const float *src0, const float *mask, const uint size, const float alpha)
{
	replaceBlendMaskRGBSrc0Alpha (dst, src0, mask, size, alpha);
}

// ***************************************************************************

template<class A>
inline void templateBlendSrc0Alpha (float *dst, const float *src0, const float *src1, const uint size, const float alpha, const A &transfo)
{
	templateBlendRGBSrc0Alpha (dst, src0, src1, size, alpha, transfo);
}

// ***************************************************************************

template<class A>
inline void templateBlendMaskSrc0Alpha (float *dst, const float *src0, const float *src1, const float *mask, const uint size, const float alpha, const A &transfo)
{
	templateBlendMaskRGBSrc0Alpha<A> (dst, src0, src1, mask, size, alpha, transfo);
}

// ***************************************************************************
// Const manipulators
// ***************************************************************************

// For i in [0~size] : dst[i].channel = color.channel
inline void replaceConst (float *dst, const float *color, const uint size, const TChannel channel)
{
	switch (channel)
	{
	case ChannelAll:
		replaceConstRGBA(dst, color, size);
		return;
	case ChannelRGB:
		replaceConstRGB(dst, color, size);
		return;
	case ChannelAlpha:
		replaceConstA(dst, color[3], size);
		return;
	}
}

// ***************************************************************************

template<class A>
inline void templateConst (float *dst, const float *src, const float *color, const uint size, const TChannel channel, const A &transfo)
{
	switch (channel)
	{
	case ChannelAll:
		templateConstRGBA<A> (dst, src, color, size, transfo);
		return;
	case ChannelRGB:
		templateConstRGB<A> (dst, src, color, size, transfo);
		return;
	case ChannelAlpha:
		templateConstA<A> (dst, src, color[3], size, transfo);
		return;
	default:
		break;
	}
}

// ***************************************************************************
// Blend const manipulators
// ***************************************************************************

// For i in [0~size] : dst[i].channel = color.channel * alpha + dst[i].channel * (1 - alpha)
inline void replaceBlendConst (float *dst, const float *color, const uint size, const TChannel channel, const float alpha)
{
	switch (channel)
	{
	case ChannelAll:
		replaceBlendConstRGBA(dst, color, size, alpha);
		return;
	case ChannelRGB:
		replaceBlendConstRGB(dst, color, size, alpha);
		return;
	case ChannelAlpha:
		replaceBlendConstA(dst, color[3], size, alpha);
		return;
	}
}

// ***************************************************************************

template<class A>
inline void templateBlendConst (float *dst, const float *src, const float *color, const uint size, const TChannel channel, const float alpha, const A &transfo)
{
	switch (channel)
	{
	case ChannelAll:
		templateBlendConstRGBA (dst, src, color, size, alpha, transfo);
		return;
	case ChannelRGB:
		templateBlendConstRGB (dst, src, color, size, alpha, transfo);
		return;
	case ChannelAlpha:
		templateBlendConstA (dst, src, color[3], size, alpha, transfo);
		return;
	}
}

// ***************************************************************************
// Mode manipulators
// ***************************************************************************

#define TRANSFORM(a) \
	case a:\
		{\
			templateTransfo (dst, src0, src1, size, channel, C##a());\
			return;\
		}

// For i in [0~size] : dst[i].channel = blendMode(src0[i].channel, src1[i].channel)
inline void transform (float *dst, const float *src0, const float *src1, uint size, TBlendMode mode, TChannel channel)
{
	switch (mode)
	{
	case Replace:
		{
			replace(dst, src0, size, channel);
			return;
		}
	TRANSFORM (Mul)
	TRANSFORM (Add)
	TRANSFORM (Sub)
	TRANSFORM (RSub)
	TRANSFORM (Screen)
	TRANSFORM (Darken)
	TRANSFORM (Lighten)
	TRANSFORM (Difference)
	TRANSFORM (Negation)
	TRANSFORM (Exclusion)
	TRANSFORM (Overlay)
	TRANSFORM (HardLight)
	TRANSFORM (SoftLight)
	TRANSFORM (ColorDodge)
	TRANSFORM (RColorDodge)
	TRANSFORM (ColorBurn)
	TRANSFORM (RColorBurn)
	TRANSFORM (Invert)
	}
}

// ***************************************************************************

#define TRANSFORM_BLEND(a) \
	case a:\
		{\
			if (mask) \
				templateBlendMask (dst, src0, src1, mask, size, channel, alpha, C##a());\
			else \
				templateBlend (dst, src0, src1, size, channel, alpha, C##a());\
			return;\
		}

// For i in [0~size] : dst[i].channel = blendMode(src0[i].channel, src0[i].channel) * alpha + dst[i].channel * (1-alpha)
inline void transformBlend (float *dst, const float *src0, const float *src1, const float *mask, uint size, TBlendMode mode, TChannel channel, float alpha)
{
	if ((alpha >= (1.f-FLT_EPSILON)) && (mask == NULL))
	{
		transform (dst, src0, src1, size, mode, channel);
	}
	else if ((alpha > FLT_EPSILON) || (mask != NULL))
	{
		switch (mode)
		{
		case Replace:
			{
				if (mask)
					replaceBlendMask (dst, src0, mask, size, channel, alpha);
				else
					replaceBlend (dst, src0, size, channel, alpha);
				return;
			}
		TRANSFORM_BLEND (Mul)
		TRANSFORM_BLEND (Add)
		TRANSFORM_BLEND (Sub)
		TRANSFORM_BLEND (RSub)
		TRANSFORM_BLEND (Screen)
		TRANSFORM_BLEND (Darken)
		TRANSFORM_BLEND (Lighten)
		TRANSFORM_BLEND (Difference)
		TRANSFORM_BLEND (Negation)
		TRANSFORM_BLEND (Exclusion)
		TRANSFORM_BLEND (Overlay)
		TRANSFORM_BLEND (HardLight)
		TRANSFORM_BLEND (SoftLight)
		TRANSFORM_BLEND (ColorDodge)
		TRANSFORM_BLEND (RColorDodge)
		TRANSFORM_BLEND (ColorBurn)
		TRANSFORM_BLEND (RColorBurn)
		TRANSFORM_BLEND (Invert)
		}
	}
}

// ***************************************************************************

#define TRANSFORM_BLEND_SRC0_ALPHA(a) \
	case a:\
		{\
			if (mask) \
				templateBlendMaskSrc0Alpha (dst, src0, src1, mask, size, alpha, C##a());\
			else \
				templateBlendSrc0Alpha (dst, src0, src1, size, alpha, C##a());\
			return;\
		}

// For i in [0~size] : dst[i].RGB = blendMode(src0[i].RGB, src0[i].RGB) * (alpha * src0[i].A) + dst[i].RGB * (1-(alpha * src0[i].A))
inline void transformBlendSrc0Alpha (float *dst, const float *src0, const float *src1, const float *mask, uint size, TBlendMode mode, float alpha)
{
	switch (mode)
	{
	case Replace:
		{
			if (mask)
				replaceBlendMaskSrc0Alpha (dst, src0, mask, size, alpha);
			else
				replaceBlendSrc0Alpha (dst, src0, size, alpha);
			return;
		}
	TRANSFORM_BLEND_SRC0_ALPHA (Mul)
	TRANSFORM_BLEND_SRC0_ALPHA (Add)
	TRANSFORM_BLEND_SRC0_ALPHA (Sub)
	TRANSFORM_BLEND_SRC0_ALPHA (RSub)
	TRANSFORM_BLEND_SRC0_ALPHA (Screen)
	TRANSFORM_BLEND_SRC0_ALPHA (Darken)
	TRANSFORM_BLEND_SRC0_ALPHA (Lighten)
	TRANSFORM_BLEND_SRC0_ALPHA (Difference)
	TRANSFORM_BLEND_SRC0_ALPHA (Negation)
	TRANSFORM_BLEND_SRC0_ALPHA (Exclusion)
	TRANSFORM_BLEND_SRC0_ALPHA (Overlay)
	TRANSFORM_BLEND_SRC0_ALPHA (HardLight)
	TRANSFORM_BLEND_SRC0_ALPHA (SoftLight)
	TRANSFORM_BLEND_SRC0_ALPHA (ColorDodge)
	TRANSFORM_BLEND_SRC0_ALPHA (RColorDodge)
	TRANSFORM_BLEND_SRC0_ALPHA (ColorBurn)
	TRANSFORM_BLEND_SRC0_ALPHA (RColorBurn)
	TRANSFORM_BLEND_SRC0_ALPHA (Invert)
	}
}

// ***************************************************************************

#define TRANSFORM_CONST(a) \
	case a:\
		{\
			templateConst (dst, src, color, size, channel, C##a());\
			return;\
		}

// For i in [0~size] : dst[i].channel = blendMode(src[i].channel, color.channel)
inline void transformConst (float *dst, const float *src, const float *color, uint size, TBlendMode mode, TChannel channel)
{
	switch (mode)
	{
	case Replace:
		{
			replaceConst(dst, color, size, channel);
			return;
		}
	TRANSFORM_CONST (Mul)
	TRANSFORM_CONST (Add)
	TRANSFORM_CONST (Sub)
	TRANSFORM_CONST (RSub)
	TRANSFORM_CONST (Screen)
	TRANSFORM_CONST (Darken)
	TRANSFORM_CONST (Lighten)
	TRANSFORM_CONST (Difference)
	TRANSFORM_CONST (Negation)
	TRANSFORM_CONST (Exclusion)
	TRANSFORM_CONST (Overlay)
	TRANSFORM_CONST (HardLight)
	TRANSFORM_CONST (SoftLight)
	TRANSFORM_CONST (ColorDodge)
	TRANSFORM_CONST (RColorDodge)
	TRANSFORM_CONST (ColorBurn)
	TRANSFORM_CONST (RColorBurn)
	TRANSFORM_CONST (Invert)
	}
}

// ***************************************************************************

#define TRANSFORM_BLEND_CONST(a) \
	case a:\
		{\
			templateBlendConst (dst, src, color, size, channel, alpha, C##a());\
			return;\
		}

// For i in [0~size] : dst[i].channel = blendMode(src[i].channel, color.channel) * alpha + dst[i].channel * (1-alpha)
inline void transformBlendConst (float *dst, const float *src, const float *color, uint size, TBlendMode mode, TChannel channel, float alpha)
{
	if (alpha >= (1.f-FLT_EPSILON))
	{
		transformConst (dst, src, color, size, mode, channel);
	}
	else if (alpha > FLT_EPSILON)
	{
		switch (mode)
		{
		case Replace:
			{
				replaceBlendConst (dst, src, size, channel, alpha);
				return;
			}
		TRANSFORM_BLEND_CONST (Mul)
		TRANSFORM_BLEND_CONST (Add)
		TRANSFORM_BLEND_CONST (Sub)
		TRANSFORM_BLEND_CONST (RSub)
		TRANSFORM_BLEND_CONST (Screen)
		TRANSFORM_BLEND_CONST (Darken)
		TRANSFORM_BLEND_CONST (Lighten)
		TRANSFORM_BLEND_CONST (Difference)
		TRANSFORM_BLEND_CONST (Negation)
		TRANSFORM_BLEND_CONST (Exclusion)
		TRANSFORM_BLEND_CONST (Overlay)
		TRANSFORM_BLEND_CONST (HardLight)
		TRANSFORM_BLEND_CONST (SoftLight)
		TRANSFORM_BLEND_CONST (ColorDodge)
		TRANSFORM_BLEND_CONST (RColorDodge)
		TRANSFORM_BLEND_CONST (ColorBurn)
		TRANSFORM_BLEND_CONST (RColorBurn)
		TRANSFORM_BLEND_CONST (Invert)
		}
	}
}

// ***************************************************************************

inline void gradient (float *dst, const std::vector<CGradientColor> &gradient, float offset)
{
	// Find left and right bounds
	std::vector<CGradientColor>::const_iterator left = gradient.end();
	std::vector<CGradientColor>::const_iterator right = gradient.begin();
	while ((right != gradient.end()) && (right->Pos < offset))
	{
		left = right;
		right++;
	}

	// Left ?
	if (left == gradient.end())
	{
		if (right == gradient.end())
			copy(dst, ColorBlack);
		else
			copy(dst, right->Color);
	}
	else
	{
		if (right == gradient.end())
			copy(dst, left->Color);
		else
		{
			if (right->Pos == left->Pos)
				copy(dst, left->Color);
			else
			{
				float blend = (offset - left->Pos) / (right->Pos - left->Pos);
				float blend_inv = 1.f - blend;
				float tangent = 1.f - left->RightPos;
				const float ab = tangent * blend;
				const float bc = tangent * blend_inv + blend;
				blend = ab * blend_inv + bc * blend;
				if (left->RightQuadratic)
					blend = easineasout (blend);
				blend_inv = 1.f - blend;
				dst[0] = left->Color[0] * blend_inv + right->Color[0] * blend;
				dst[1] = left->Color[1] * blend_inv + right->Color[1] * blend;
				dst[2] = left->Color[2] * blend_inv + right->Color[2] * blend;
				dst[3] = left->Color[3] * blend_inv + right->Color[3] * blend;
			}
		}
	}
}

// ***************************************************************************
// Random
// ***************************************************************************

inline TRandomValue getRandomValue (float value)
{
	return (TRandomValue)(value*32767.f);
}

// ***************************************************************************

inline float getNextRandomValue (TRandomValue &previous)
{
	return (float)(((previous = previous * 214013L + 2531011L) >> 16) & 0x7fff)/32767.f;
}

// ***************************************************************************
// Gaussian
// ***************************************************************************

inline void initGaussian (float radius, float *values, uint size)
{
	uint dx, dy;
	double sd, c1, c2;

	/* This formula isn't really correct, but it'll do */
	sd = radius / 3.329042969;
	c1 = 1.0 / sqrt(2.0 * NLMISC::Pi * sd);
	c2 = -2.0 * (sd * sd);
	
	for (dy=0; dy<size; dy++)
	{
		for (dx=dy; dx<size; dx++)
		{
			uint index = dx*size+dy;
			values[index] = (float)(c1 * (exp(((dx*dx)+ (dy*dy))/ c2)));
			values[dy*size+dx] = values[index];
		}
	}
}

// ***************************************************************************
// Bilinear
// ***************************************************************************

inline float easineasout(float x)
{
	float y;
	// cubic tq f(0)=0, f'(0)=0, f(1)=1, f'(1)=0.
	float x2=x*x;
	float x3=x2*x;
	y= -2*x3 + 3*x2;
	return y;
}

// ***************************************************************************

inline void bilinearRGB (float *dst, const float *p00, const float *p01, const float *p10, const float *p11, 
						 const float x, const float y)
{
	float x2 = easineasout(x);
	float y2 = easineasout(y);
	const float _1_x = 1.f - x2;
	const float _1_y = 1.f - y2;
	dst[0] = (p00[0] * _1_x + p01[0] * x2) * _1_y + (p10[0] * _1_x + p11[0] * x2) * y2;
	dst[1] = (p00[1] * _1_x + p01[1] * x2) * _1_y + (p10[1] * _1_x + p11[1] * x2) * y2;
	dst[2] = (p00[2] * _1_x + p01[2] * x2) * _1_y + (p10[2] * _1_x + p11[2] * x2) * y2;
}

inline void bilinearSampleTileRGB (float *dst, const float *src, float x, float y, uint widthMask, 
						   uint heightMask, uint widthShift, bool tileX, bool tileY)
{
	// X
	float tmp = (float)floor (x);
	x -= tmp;
	sint ix = (sint)tmp;
	sint ix2 = ix+1;
	if (tileX)
	{
		ix &= widthMask;
		ix2 &= widthMask;
	}
	else
	{
		NLMISC::clamp (ix, (sint)0, (sint)widthMask);
		NLMISC::clamp (ix2, (sint)0, (sint)widthMask);
	}

	// Y
	tmp = (float)floor (y);
	y -= tmp;
	sint iy = (sint)tmp;
	sint iy2 = iy+1;
	if (tileY)
	{
		iy &= heightMask;
		iy2 &= heightMask;
	}
	else
	{
		NLMISC::clamp (iy, (sint)0, (sint)heightMask);
		NLMISC::clamp (iy2, (sint)0, (sint)heightMask);
	}
	ix <<= 2;
	ix2 <<= 2;
	iy <<= widthShift+2;
	iy2 <<= widthShift+2;

	bilinearRGB (dst, src+ix+iy, src+ix2+iy, src+ix+iy2, src+ix2+iy2, x, y);
}

// ***************************************************************************

inline void bilinearRGB (float *dst, const float *p0, const float *p1, const float x)
{
	float x2 = easineasout(x);
	const float _1_x = 1.f - x2;
	dst[0] = (p0[0] * _1_x + p1[0] * x2);
	dst[1] = (p0[1] * _1_x + p1[1] * x2);
	dst[2] = (p0[2] * _1_x + p1[2] * x2);
}

// ***************************************************************************

inline void bilinearSampleTileRGB (float *dst, const float *src, float x, uint widthMask, bool tileX)
{
	// X
	float tmp = (float)floor (x);
	x -= tmp;
	sint ix = (sint)tmp;
	sint ix2 = ix+1;
	if (tileX)
	{
		ix &= widthMask;
		ix2 &= widthMask;
	}
	else
	{
		NLMISC::clamp (ix, (sint)0, (sint)widthMask);
		NLMISC::clamp (ix2, (sint)0, (sint)widthMask);
	}

	ix <<= 2;
	ix2 <<= 2;

	bilinearRGB (dst, src+ix, src+ix2, x);
}

// ***************************************************************************

template<class A>
inline void copyPixels (float *dst, uint dstWidth, uint dstHeight, sint dstX, sint dstY, const float *src, uint srcWidth, 
				 uint srcHeight, bool tileX, bool tileY, const A &transfo)
{
	// *********
	// * 0 * 2 *
	// *********
	// * 1 * 3 *
	// *********

	const uint dstWidthMask = dstWidth-1;
	const uint dstHeightMask = dstHeight-1;

	uint srcHeightLeft = srcHeight;
	sint y = dstY;
	sint srcY = 0;
	while (srcHeightLeft)
	{
		const uint heightToCopy = std::min(srcHeightLeft, dstHeight - (y&dstHeightMask));

		if (tileY || ((y>=0) && (y<(sint)dstHeight)))
		{
			uint srcWidthLeft = srcWidth;
			sint x = dstX;
			sint srcX = 0;
			while (srcWidthLeft)
			{
				const uint widthToCopy = std::min(srcWidthLeft, dstWidth - (x&dstWidthMask));

				if (tileX || ((x>=0) && (x<(sint)dstWidth)))
				{
					uint i;
					for (i=0; i<heightToCopy; i++)
					{
						float *dstTemp = dst + ((((y+i)&dstHeightMask)*dstWidth + (x&dstWidthMask)) * 4);
						const float *srcTemp = src + ((((srcY+i)*srcWidth) + srcX) * 4);
						transfo.transform (dstTemp, dstTemp, srcTemp, widthToCopy*4);
					}
				}

				// Remove some pixels
				srcWidthLeft -= widthToCopy;
				srcX += widthToCopy;

				// x to the start of the next
				x &= ~dstWidthMask;
				x += dstWidth;
			}
		}

		// Remove some pixels
		srcHeightLeft -= heightToCopy;
		srcY += heightToCopy;

		// y to the start of the next
		y &= ~dstHeightMask;
		y += dstHeight;
	}
}

// ***************************************************************************

template<class A>
inline void addPixels (float *dst, uint dstWidth, uint dstHeight, sint dstX, sint dstY, const float *src, uint srcWidth, 
				 uint srcHeight, bool tileX, bool tileY, float *colorToTransform, const A &transfo)
{
	// *********
	// * 0 * 2 *
	// *********
	// * 1 * 3 *
	// *********

	const uint dstWidthMask = dstWidth-1;
	const uint dstHeightMask = dstHeight-1;

	uint srcHeightLeft = srcHeight;
	sint y = dstY;
	sint srcY = 0;
	while (srcHeightLeft)
	{
		const uint heightToCopy = std::min(srcHeightLeft, dstHeight - (y&dstHeightMask));

		if (tileY || ((y>=0) && (y<(sint)dstHeight)))
		{
			uint srcWidthLeft = srcWidth;
			sint x = dstX;
			sint srcX = 0;
			while (srcWidthLeft)
			{
				const uint widthToCopy = std::min(srcWidthLeft, dstWidth - (x&dstWidthMask));

				if (tileX || ((x>=0) && (x<(sint)dstWidth)))
				{
					uint i;
					for (i=0; i<heightToCopy; i++)
					{
						float *dstTemp = dst + ((((y+i)&dstHeightMask)*dstWidth + (x&dstWidthMask)) * 4);
						const float *srcTemp = src + ((((srcY+i)*srcWidth) + srcX) * 4);
						templateConstAddRGBA (dstTemp, srcTemp, colorToTransform, widthToCopy*4, transfo);
					}
				}

				// Remove some pixels
				srcWidthLeft -= widthToCopy;
				srcX += widthToCopy;

				// x to the start of the next
				x &= ~dstWidthMask;
				x += dstWidth;
			}
		}

		// Remove some pixels
		srcHeightLeft -= heightToCopy;
		srcY += heightToCopy;

		// y to the start of the next
		y &= ~dstHeightMask;
		y += dstHeight;
	}
}

// ***************************************************************************

} // NLTEXGEN

#endif // NL_TRANSFORM_BLEND

/* End of transform_float.h */
