//*****************************************************************************
//
// $Created: FBD 03-04-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Line.h $
// $Author: Age $
// $Modtime: 14/03/05 14:14 $
// $Revision: 3 $
// $Workfile: MOS_Line.h $
//
//*****************************************************************************

#ifndef __MOS_Line_h_
#define __MOS_Line_h_

#include "MOS_Node.h"


// =============================================================================
/** @class  MOS_Line
    @brief  Geometry element of a map object.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_Line
{
public:
    typedef std::vector< MOS_Line >      T_LineVector;
    typedef T_LineVector::iterator       IT_LineVector;

public:
    //-------------------------------------------------------------------------
    /** @name Constructor*/
    //-------------------------------------------------------------------------
    //@{
     MOS_Line();
    ~MOS_Line();
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MT_Line         GetLine();
    const MOS_Node& GetPosStart() const;
    const MOS_Node& GetPosEnd() const;
    //@}

private:
    MOS_Node* pNode1_;
    MOS_Node* pNode2_;
};


#ifdef MOS_USE_INLINE
#	include "MOS_Line.inl"
#endif

#endif // __MOS_Line_h_