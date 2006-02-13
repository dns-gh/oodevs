//*****************************************************************************
//
// $Created: FBD 03-04-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Line.h $
// $Author: Age $
// $Modtime: 14/03/05 14:14 $
// $Revision: 3 $
// $Workfile: Line.h $
//
//*****************************************************************************

#ifndef __Line_h_
#define __Line_h_

#include "Node.h"


// =============================================================================
/** @class  Line
    @brief  Geometry element of a map object.
*/
// Created: APE 2004-07-19
// =============================================================================
class Line
{
public:
    typedef std::vector< Line >      T_LineVector;
    typedef T_LineVector::iterator       IT_LineVector;

public:
    //-------------------------------------------------------------------------
    /** @name Constructor*/
    //-------------------------------------------------------------------------
    //@{
     Line();
    ~Line();
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MT_Line         GetLine();
    const Node& GetPosStart() const;
    const Node& GetPosEnd() const;
    //@}

private:
    Node* pNode1_;
    Node* pNode2_;
};


#	include "Line.inl"

#endif // __Line_h_