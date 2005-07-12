
// Name   : MOS_Tri.h
// Created: FBD 02-07-01
//*****************************************************************************

#ifndef __MOS_Tri_h_
#define __MOS_Tri_h_

#include "MOS_Types.h"
#include "MOS_Node.h"

//=============================================================================
// Created: FBD 02-07-01
//=============================================================================
class MOS_Tri
{

public:
    //-------------------------------------------------------------------------
    /** @name Constructor*/
    //-------------------------------------------------------------------------
    //@{
    MOS_Tri();
    virtual ~MOS_Tri();
    MOS_Tri( const MOS_Tri& rhs );
    MOS_Tri& operator = ( const MOS_Tri& rhs );
    //@}

    //-------------------------------------------------------------------------
    /** @name Neighbors*/
    //-------------------------------------------------------------------------
    //@{
    MT_Vector2D        GetCenMOS    () const;
    bool               Intersect2D  ( const MT_Line& line ) const;
    bool               IsInside     ( const MT_Vector2D& vPos, MT_Float rWeldValue = 0.0 ) const;
    const MOS_Node&    GetPos1      () const;
	const MOS_Node&    GetPos2      () const;
	const MOS_Node&    GetPos3      () const;
    uint               GetFrame     () const;
    void               SetFrame     ( uint nFrame );
    MOS_Node&          GetPos1      ();
	MOS_Node&          GetPos2      ();
	MOS_Node&          GetPos3      ();
    MT_Float           GetSize      () const;
    //@}
    
    //-------------------------------------------------------------------------
    /** @name IN/OUT*/
    //-------------------------------------------------------------------------
    //@{
    int Read( MT_InputArchive_ABC& archive );
    //@}

private:
    uint                    nFrame_;

    MOS_Node*               pNode1_;
    MOS_Node*               pNode2_;
    MOS_Node*               pNode3_;

    MT_Float                rSize_;
};

typedef std::vector< MOS_Tri >            T_TriVector;
typedef const T_TriVector               CT_TriVector;
typedef T_TriVector::iterator           IT_TriVector;
typedef T_TriVector::const_iterator     CIT_TriVector;


#   include "MOS_Tri.inl"

#endif // __MOS_Tri_h_
