// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-02 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MapDropEvent.h $
// $Author: Ape $
// $Modtime: 3/09/04 11:24 $
// $Revision: 1 $
// $Workfile: MapDropEvent.h $
//
// *****************************************************************************

#ifndef __MapDropEvent_h_
#define __MapDropEvent_h_

#ifdef __GNUG__
#   pragma interface
#endif

// =============================================================================
/** @class  MapDropEvent
    @brief  MapDropEvent
    @par    Using example
    @code
    MapDropEvent;
    @endcode
*/
// Created: APE 2004-09-02
// =============================================================================
class MapDropEvent : public QDropEvent
{
    MT_COPYNOTALLOWED( MapDropEvent );

public:
    //! @name Constructors/Destructor
    //@{
     MapDropEvent( const QDropEvent& dropEvent, const MT_Vector2D& vMapPos );
    ~MapDropEvent();
    //@}

    //! @name Operations
    //@{
    const MT_Vector2D& GetMapPos() const;
    //@}

private:
    //! @name Member data
    //@{
    MT_Vector2D vMapPos_;
    //@}
};

#include "MapDropEvent.inl"

#endif // __MapDropEvent_h_
