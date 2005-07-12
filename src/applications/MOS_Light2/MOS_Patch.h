//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Patch.h $
// $Author: Ape $
// $Modtime: 15/09/04 17:47 $
// $Revision: 3 $
// $Workfile: MOS_Patch.h $
//
//*****************************************************************************

#ifndef __MOS_Patch_h_
#define __MOS_Patch_h_

class MOS_ShapePatch;


// =============================================================================
/** @class  MOS_Patch
    @brief  A rectangular subdivision of the map.
    @par    To speed up the rendering process, the map is split up in multiple
            MOS_Patch, and a patch is only drawn if it's on screen. Each patch
            has a list of pointers to the MOS_ShapePatch that are on it and which
            are containers for the part of an object's geometry that's on a patch
            (i.e. an object owns a MOS_ShapePatch for each patch is spans on).
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_Patch
{
    friend class MOS_GLTool;

public:
     MOS_Patch();
    ~MOS_Patch();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Read( MT_InputArchive_ABC& archive );
    //@}

    //-------------------------------------------------------------------------
    /** @name Shape patchs */
    //-------------------------------------------------------------------------
    //@{
    void RegisterShapePatch  ( MOS_ShapePatch& shapePatch );
    void UnregisterShapePatch( MOS_ShapePatch& shapePatch );
    //@}

private:
    typedef std::vector< MOS_ShapePatch* >      T_ShapePatchVector;
    typedef T_ShapePatchVector::iterator        IT_ShapePatchVector;
    typedef T_ShapePatchVector::const_iterator  CIT_ShapePatchVector;

private:
    MT_Rect rect_;

    T_ShapePatchVector shapePatchVector_;

    int nGl_;
};


#ifdef MOS_USE_INLINE
#	include "MOS_Patch.inl"
#endif

#endif // __MOS_Patch_h_