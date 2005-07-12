//*****************************************************************************
// Name   : MOS_PatchTexture.h
// Created: FBD 02-02-27
//*****************************************************************************

#ifndef __MOS_PatchTexture_h_
#define __MOS_PatchTexture_h_

#include "MOS_Types.h"

class MOS_ShapePatch;
class MOS_GLWidget;

#define MOS_BAD_VAL_ALTITUDE -1

//=============================================================================
// Created: FBD 02-02-27
//=============================================================================
class MOS_PatchTexture
{
    MT_COPYNOTALLOWED( MOS_PatchTexture )

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::vector< MOS_ShapePatch* >   T_ShapePatchPtrVector;
    typedef T_ShapePatchPtrVector::iterator  IT_ShapePatchPtrVector;
    typedef T_ShapePatchPtrVector::const_iterator  CIT_ShapePatchPtrVector;
    //@}

public:
    explicit MOS_PatchTexture();
    explicit MOS_PatchTexture( const MT_Rect& rect, uint nIdxPatchX, uint nIdxPatchY );
    virtual ~MOS_PatchTexture();
  
    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    uint           GetIdxPatchX() const;
    uint           GetIdxPatchY() const;
    const MT_Rect& GetRect     () const;
    MT_Float       GetHandle   () const;
    uint32         GetFrame       () const;
    void           SetFrame       ( uint nFrame );
    //@}

   
   //-------------------------------------------------------------------------
    /** @name Geometry */
    //-------------------------------------------------------------------------
    //@{
    bool IsInside   ( const MT_Vector2D& point );
	bool Intersect2D( const MT_Line& line );
    bool Intersect2D( const MT_Triangle& triangle );
    //@}

   //-------------------------------------------------------------------------
    /** @name Patchs */
    //-------------------------------------------------------------------------
    //@{
    void                       LinkToNeighbors();
    MOS_PatchTexture*          GetPatchUp   ();
	MOS_PatchTexture*          GetPatchRight();
	MOS_PatchTexture*          GetPatchDown ();
	MOS_PatchTexture*          GetPatchLeft ();
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX */
    //-------------------------------------------------------------------------
    //@{
    void            InitializeGFX   ();
    void            TerminateGFX    ();
    bool            IsOnScreen      ();
    MOS_GLWidget*   GetGLWidget     () const;
    void            SetGLWidget     ( MOS_GLWidget* pGlWidget );
    void            Draw            ( MT_Float rDistPercent );
    void            GenerateAltitude();
    void            GenerateCouverture();
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Patchs */
    //-------------------------------------------------------------------------
    //@{
    MT_Rect                rect_;

    uint                   nIdxPatchX_;
    uint                   nIdxPatchY_;
    MT_Float               rHandlePatch_;
    
    MOS_PatchTexture*	   pPatchUp_;
	MOS_PatchTexture*	   pPatchRight_;
	MOS_PatchTexture*	   pPatchDown_;
	MOS_PatchTexture*	   pPatchLeft_;
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX */
    //-------------------------------------------------------------------------
    //@{
    GLuint                  glRect_;
    MOS_GLWidget*           pGlWidget_;
    GLuint                  nGLPatchAltitude_;	
	GLuint                  nGLTextureAltitude_[1];
    GLuint                  nGLPatchCouverture_;	
	GLuint                  nGLTextureCouverture_[1];
    uint                    nFrame_;
    //@}
};

#   include "MOS_PatchTexture.inl"

#endif // __MOS_PatchTexture_h_
