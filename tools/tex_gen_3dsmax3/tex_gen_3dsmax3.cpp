/**********************************************************************
 *<
	FILE: tex_gen_3dsmax3.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 1997, All Rights Reserved.
 **********************************************************************/

#include "tex_gen_3dsmax3.h"

#undef max
#undef min

#include "nel/misc/rgba.h"
#include "nel/misc/file.h"
#include "tex_gen/basics.h"
#include "tex_gen/tex_gen_document.h"
#include "tex_gen/bitmap_float.h"
#include "tex_gen/tex_gen_op.h"

#define _tex_gen_3dsmax3_CLASS_ID	Class_ID(0x391acadf, 0x48bfce7c)

#define PBLOCK_REF	0

using namespace NLMISC;
using namespace NLTEXGEN;



class _tex_gen_3dsmax3 : public BitmapIO {
	public:
		// Parameter block
		IParamBlock2	*pblock;	//ref 0
		
		// Loading/Saving
		IOResult Load(ILoad *iload);
		IOResult Save(ISave *isave);

		//From Animatable
		Class_ID ClassID() {return _tex_gen_3dsmax3_CLASS_ID;}		
		SClass_ID SuperClassID() { return BMM_IO_CLASS_ID; }
		void GetClassName(TSTR& s) {s = GetString(IDS_CLASS_NAME);}
		
		RefTargetHandle Clone( RemapDir &remap );
		RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
			PartID& partID,  RefMessage message);

		int NumSubs() { return 1; }
		TSTR SubAnimName(int i) { return GetString(IDS_PARAMS); }				
		Animatable* SubAnim(int i) { return pblock; }
		int NumRefs() { return 1; }
		RefTargetHandle GetReference(int i) { return pblock; }
		void SetReference(int i, RefTargetHandle rtarg) { pblock=(IParamBlock2*)rtarg; }

		int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
		IParamBlock2* GetParamBlock(int i) { return pblock; } // return i'th ParamBlock
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock
		void DeleteThis() { delete this; }		
		
		// Implementation
		int				ExtCount() { return 1; }
		const TCHAR		*Ext( int n ) { return "texgen"; }
		const TCHAR		*LongDesc( ) { return "NeL Texture Generator Image File"; }
		const TCHAR		*ShortDesc() { return "NeL Tex Gen"; }
		const TCHAR		*AuthorName() { return "Cyril \"Hulud\" Corvazier"; }
		const TCHAR		*CopyrightMessage() { return "This code is released under GNU GPL, Digital Murder 2003"; }
		UINT			Version() { return 1; }
		int				Capability() { return BMMIO_READER | BMMIO_EXTENSION; }
		void			ShowAbout( HWND hWnd ) { MessageBox (hWnd, "NeL TexGen Image Loader by Cyril \"Hulud\" Corvazier for Digital Murder 2003\nRelease under GNU GPL", "About NeL TexGen For 3dsmax", MB_OK|MB_ICONINFORMATION); }
		DWORD			EvaluateConfigure( ) { return 0; }
		BOOL			LoadConfigure( void *ptr ) { return TRUE; }
		BOOL			SaveConfigure( void *ptr ) { return TRUE; }
		BMMRES			GetImageInfo( BitmapInfo *bi )
		{
			bi->SetWidth(256);
			bi->SetHeight(256);
			bi->SetType(BMM_TRUE_64);
			bi->SetAspect(1.0f);
			bi->SetGamma(1.0f);
			bi->SetFirstFrame(0);
			bi->SetLastFrame(0);
			return BMMRES_SUCCESS;		
		}
		BitmapStorage	*Load( BitmapInfo *bi, Bitmap *map, BMMRES *status )
		{
			openMode = BMM_OPEN_R;

			CIFile file;
			if (file.open (bi->Filename()))
			{
				try
				{
					NLTEXGEN::CTexGenDocument	doc;
					file.serial(doc);

					CFloatBitmap bitmap;
					CRenderParameter renderParameters (false, false, false);
					doc.Operators[0]->eval (bitmap, renderParameters);
					if ((bitmap.getWidth() != 0) && (bitmap.getHeight() != 0))
					{
						const float *pixels = bitmap.getPixels();
						bi->SetWidth(bitmap.getWidth());
						bi->SetHeight(bitmap.getHeight());
						bi->SetType(BMM_TRUE_64);
						bi->SetAspect(1.0f);
						bi->SetGamma(1.0f);
						bi->SetFirstFrame(0);
						bi->SetLastFrame(0);

						BitmapStorage *s = BMMCreateStorage(map->Manager(), BMM_TRUE_32);
						if (s->Allocate(bi,map->Manager(), BMM_OPEN_R)==0) 
						{
							if(s) 
							{
								delete s;
								s = NULL;
							}
							return NULL;
						}

						uint y;
						BMM_Color_64 *line = new BMM_Color_64[bitmap.getWidth()];
						for (y=0; y<bitmap.getHeight(); y++)
						{
							uint x;
							for (x=0; x<bitmap.getWidth(); x++, pixels+=4)
							{
								float r = 65535.f * pixels[0];
								clamp (r, 0.f, 65535.f);
								float g = 65535.f * pixels[1];
								clamp (g, 0.f, 65535.f);
								float b = 65535.f * pixels[2];
								clamp (b, 0.f, 65535.f);
								float a = 65535.f * pixels[3];
								clamp (a, 0.f, 65535.f);
								line[x].r = (uint16)r;
								line[x].g = (uint16)g;
								line[x].b = (uint16)b;
								line[x].a = (uint16)a;
							}
							s->PutPixels(0, y, bitmap.getWidth(), line);
						}
						delete line;

						s->bi.CopyImageInfo(bi);
						
						return s;
					}
					else
					{
						*status = BMMRES_INVALIDFORMAT;
					}
				}
				catch (Exception &)
				{
					*status = BMMRES_CORRUPTFILE;
				}
			}
			else
			{
				*status = BMMRES_FILENOTFOUND;
			}
			return NULL;
		}

		//Constructor/Destructor
		_tex_gen_3dsmax3() 
		{
			static bool initialized = false;
			if (!initialized)
			{
				registerTypes();
				initialized = true;
			}
		};
		~_tex_gen_3dsmax3() {};		
};


class _tex_gen_3dsmax3ClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic() {return 1;}
	void *			Create(BOOL loading = FALSE) {return new _tex_gen_3dsmax3();}
	const TCHAR *	ClassName() {return GetString(IDS_CLASS_NAME);}
	SClass_ID		SuperClassID() {return BMM_IO_CLASS_ID;}
	Class_ID		ClassID() {return _tex_gen_3dsmax3_CLASS_ID;}
	const TCHAR* 	Category() {return GetString(IDS_CATEGORY);}
	const TCHAR*	InternalName() { return _T("_tex_gen_3dsmax3"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle
};

static _tex_gen_3dsmax3ClassDesc _tex_gen_3dsmax3Desc;
ClassDesc2* Gettex_gen_3dsmax3Desc() {return &_tex_gen_3dsmax3Desc;}

enum { _tex_gen_3dsmax3_params };

//TODO: Add enums for various parameters
enum { 
	pb_spin,
};

static ParamBlockDesc2 _tex_gen_3dsmax3_param_blk ( _tex_gen_3dsmax3_params, _T("params"),  0, &_tex_gen_3dsmax3Desc, 
	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
	//rollout
	IDD_PANEL, IDS_PARAMS, 0, 0, NULL,
	// params
	pb_spin, 			_T("spin"), 		TYPE_FLOAT, 	P_ANIMATABLE, 	IDS_SPIN, 
		p_default, 		0.1f, 
		p_range, 		0.0f,1000.0f, 
		p_ui, 			TYPE_SPINNER,		EDITTYPE_FLOAT, IDC_EDIT,	IDC_SPIN, 0.01f, 
		end,
	end
	);


