//*****************************************************************************
//
// $Created: FBD 03-04-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Line.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_Line.h $
//
//*****************************************************************************

#ifndef __MOS_Line_h_
#define __MOS_Line_h_

#include "MOS_Types.h"
#include "MOS_Node.h"

//*****************************************************************************
// Created: FBD 03-04-08
//*****************************************************************************
class MOS_Line
{

public:
    //-------------------------------------------------------------------------
    /** @name Constructor*/
    //-------------------------------------------------------------------------
    //@{
    MOS_Line();
    MOS_Line( MOS_Node& node1, MOS_Node& node2 );
    virtual ~MOS_Line();

    MOS_Line( const MOS_Line& rhs );
    MOS_Line& operator = ( const MOS_Line& rhs );
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MT_Line         GetLine();
    const MOS_Node& GetPosStart() const;
    const MOS_Node& GetPosEnd() const;
    MOS_Node&       GetPosStart();
    MOS_Node&       GetPosEnd();
    uint            GetFrame() const;
    void            SetFrame( uint nFrame );
    bool            IsInside( const MT_Vector2D& vPos, MT_Float rWeldValue = 0.0 ) const;
    //@}

    //-------------------------------------------------------------------------
    /// @name IN/OUT
    //-------------------------------------------------------------------------
    //@{
    int Read( MT_InputArchive_ABC& archive );
    //@}


private:
    MOS_Node* pNode1_;
    MOS_Node* pNode2_;
    uint      nFrame_;
};

typedef std::vector< MOS_Line >           T_MOSLineVector;
typedef const T_MOSLineVector              CT_MOSLineVector;
typedef T_MOSLineVector::iterator          IT_MOSLineVector;
typedef T_MOSLineVector::const_iterator    CIT_MOSLineVector;

#ifdef MOS_USE_INLINE
#	include "MOS_Line.inl"
#endif


#endif // __MOS_Line_h_