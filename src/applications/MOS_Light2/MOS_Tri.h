//*****************************************************************************
// Name   : MOS_Tri.h
// Created: FBD 02-07-01
//*****************************************************************************

#ifndef __MOS_Tri_h_
#define __MOS_Tri_h_

#include "MOS_Node.h"


// =============================================================================
/** @class  MOS_Tri
    @brief  Geometry element of a map object.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_Tri
{
    friend class MOS_GLTool;

public:
    typedef std::vector< MOS_Tri >   T_TriVector;
    typedef T_TriVector::iterator    IT_TriVector;

public:
    //-------------------------------------------------------------------------
    /** @name Constructor*/
    //-------------------------------------------------------------------------
    //@{
     MOS_Tri();
    ~MOS_Tri();
    //@}

    //-------------------------------------------------------------------------
    /** @name Neighbors*/
    //-------------------------------------------------------------------------
    //@{
    const MOS_Node& GetPos1() const;
	const MOS_Node& GetPos2() const;
	const MOS_Node& GetPos3() const;
    //@}
    
    //-------------------------------------------------------------------------
    /** @name IN/OUT*/
    //-------------------------------------------------------------------------
    //@{
    void Read( MT_InputArchive_ABC& archive );
    //@}

private:
    uint                    nFrame_; // Used for drawing, accessed by friend class.

    MOS_Node*               pNode1_;
    MOS_Node*               pNode2_;
    MOS_Node*               pNode3_;
};


#ifdef  MOS_USE_INLINE
#   include "MOS_Tri.inl"
#endif

#endif // __MOS_Tri_h_
