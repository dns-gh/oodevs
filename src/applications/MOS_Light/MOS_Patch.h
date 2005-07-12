//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Patch.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_Patch.h $
//
//*****************************************************************************

#ifndef __MOS_Patch_h_
#define __MOS_Patch_h_

#include "MOS_Types.h"

#include "MT_Rect.h"

class MOS_ShapePatch;

//*****************************************************************************
// Created: FBD 02-11-21
//*****************************************************************************
class MOS_Patch
{
public:
    explicit MOS_Patch();
    virtual ~MOS_Patch();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Read( MT_InputArchive_ABC& archive );
    //@}


    //-------------------------------------------------------------------------
    /** @name Draw */
    //-------------------------------------------------------------------------
    //@{
    void DrawAltitudes();
    void DrawBorders();
    void DrawObjects( uint nFrame, MT_Float rDistPercent );
    void DrawNameObjects( uint nFrame, MT_Float rDistPercent );
    //@}

    //-------------------------------------------------------------------------
    /** @name Shape patchs */
    //-------------------------------------------------------------------------
    //@{
    void RegisterShapePatch  ( MOS_ShapePatch& shapePatch );
    void UnregisterShapePatch( MOS_ShapePatch& shapePatch );
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Vector */
    //-------------------------------------------------------------------------
    //@{
    typedef std::vector< MOS_ShapePatch* > T_ShapePatchVector;
    typedef T_ShapePatchVector::iterator   IT_ShapePatchVector;
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    void InitializeGL();
    bool IsOnScreen ();
    //@}
    
private:
    MT_Rect rect_;
    uint    nIdxPatchX_;
    uint    nIdxPatchY_;

    T_ShapePatchVector shapePatchVector_;

    GLuint  nGLPatch_;	
    GLuint  glRect_;
    uint    nFrame_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_Patch.inl"
#endif


#endif // __MOS_Patch_h_