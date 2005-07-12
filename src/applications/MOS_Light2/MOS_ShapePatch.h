//*****************************************************************************
// Name   : MOS_ShapePatch.h
// Created: FBD 02-07-01
//*****************************************************************************

#ifndef __MOS_ShapePatch_h_
#define __MOS_ShapePatch_h_

#include "MOS_Types.h"
#include "MOS_Node.h"
#include "MOS_Line.h"
#include "MOS_Tri.h"

class MOS_Object;
class MOS_Patch;


// =============================================================================
/** @class  MOS_ShapePatch
    @brief  Holds the geometry elements of a MOS_Object that rest on a MOS_Patch.
    @par    A MOS_Object owns a MOS_ShapePatch for each patch is spans on and 
            thus has it's geometry split up between patches. This is used to 
            speed up rendering, as a patch is only drawn if it's on screen. For
            example, thanks to this, a large forest does not have to be drawn
            whole if only a small portion of it is on screen.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_ShapePatch
{
    MT_COPYNOTALLOWED( MOS_ShapePatch );
    friend class MOS_GLTool;

public:
     MOS_ShapePatch( MOS_Object& object, MOS_Patch& patch );
    ~MOS_ShapePatch();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_Patch&      GetPatch() const;
    MOS_Object&     GetObjectShape() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name IN/OUT */
    //-------------------------------------------------------------------------
    //@{
    void Read( MT_InputArchive_ABC& );
    //@}

private:
    MOS_Object&             object_;
    MOS_Patch&              patch_;
    T_NodePtrVector         nodePtrVector_;
    T_MOSLinePtrVector      linePtrVector_;
    T_TriPtrVector          triPtrVector_;

    int nGL_;
};


#ifdef  MOS_USE_INLINE
#   include "MOS_ShapePatch.inl"
#endif

#endif // __MOS_ShapePatch_h_
