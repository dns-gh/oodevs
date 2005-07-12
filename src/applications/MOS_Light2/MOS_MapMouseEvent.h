// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-01 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MapMouseEvent.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:42 $
// $Revision: 3 $
// $Workfile: MOS_MapMouseEvent.h $
//
// *****************************************************************************

#ifndef __MOS_MapMouseEvent_h_
#define __MOS_MapMouseEvent_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MOS_MapMouseEvent
    @brief  Mouse event that happened on the map.
*/
// Created: APE 2004-09-01
// =============================================================================
class MOS_MapMouseEvent : public QMouseEvent
{
    MT_COPYNOTALLOWED( MOS_MapMouseEvent );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_MapMouseEvent( const QMouseEvent& mouseEvent, const MT_Vector2D& vMapPos, float rDistancePerPixel );
    ~MOS_MapMouseEvent();
    //@}

    //! @name Operations
    //@{
    const MT_Vector2D& GetMapPos() const;
    const float        GetDistancePerPixel() const;
    //@}

private:
    //! @name Member data
    //@{
    MT_Vector2D vMapPos_;
    float rDistancePerPixel_;
    //@}
};


#include "MOS_MapMouseEvent.inl"

#endif // __MOS_MapMouseEvent_h_
