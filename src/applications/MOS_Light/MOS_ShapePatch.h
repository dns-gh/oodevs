//*****************************************************************************
// Name   : MOS_ShapePatch.h
// Created: FBD 02-07-01
//*****************************************************************************

#ifndef __MOS_ShapePatch_h_
#define __MOS_ShapePatch_h_

#include "MOS_Types.h"
class MOS_Object;
class MOS_Patch;

//=============================================================================
// Created: FBD 02-07-01
//=============================================================================
class MOS_ShapePatch
{
    friend class MOS_Object;
    friend class SIM_ShapePatch; // Gfx

public:
    MOS_ShapePatch();
    MOS_ShapePatch( MOS_Object& object, MOS_Patch& patch );
    virtual ~MOS_ShapePatch();
    MOS_ShapePatch( const MOS_ShapePatch& rhs );
    MOS_ShapePatch& operator = ( const MOS_ShapePatch& rhs );

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_Patch&      GetPatch() const;
    MOS_Object&     GetObjectShape() const;
    bool            IsItArea() const;
    bool            IsItLine() const;
    bool            IsItPoint() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name IN/OUT */
    //-------------------------------------------------------------------------
    //@{
    int Read( MT_InputArchive_ABC& );
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void Draw( uint nFrame, MT_Float rDistPercent );
    void CreateTrianglesGL( GFX_Color& color, MT_Float rDistLossy, uint nFrame );
    void CreateLinesTrianglesGL( GFX_Color& color );
    void CreateBorderLinesGl( GFX_Color& color, uint nFrame );
    void CreateLinesGl( GFX_Color& color, MT_Float rSize, uint nFrame );
    void CreateLinesGlAutoroute( GFX_Color& color, MT_Float rSize, uint nFrame );
    void CreateLinesGlRailRoad( GFX_Color& color, uint nFrame );
    //@}





private:   
    MOS_Object*             pObject_;
    MOS_Patch*              pPatch_;
    T_NodePtrVector         nodePtrVector_;
    T_MOSLinePtrVector      linePtrVector_;
    T_TriPtrVector          triPtrVector_;
};

#   include "MOS_ShapePatch.inl"

#endif // __MOS_ShapePatch_h_
