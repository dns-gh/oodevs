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
// $Archive: /MVW_v10/Build/SDK/Light2/src/MapMouseEvent.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:42 $
// $Revision: 3 $
// $Workfile: MapMouseEvent.h $
//
// *****************************************************************************

#ifndef __MapMouseEvent_h_
#define __MapMouseEvent_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MapMouseEvent
    @brief  Mouse event that happened on the map.
*/
// Created: APE 2004-09-01
// =============================================================================
class MapMouseEvent : public QMouseEvent
{
    MT_COPYNOTALLOWED( MapMouseEvent );

public:
    //! @name Constructors/Destructor
    //@{
     MapMouseEvent( const QMouseEvent& mouseEvent, const MT_Vector2D& vMapPos, float rDistancePerPixel );
    ~MapMouseEvent();
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


#include "MapMouseEvent.inl"

#endif // __MapMouseEvent_h_
