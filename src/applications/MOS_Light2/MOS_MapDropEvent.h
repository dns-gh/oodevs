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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MapDropEvent.h $
// $Author: Ape $
// $Modtime: 3/09/04 11:24 $
// $Revision: 1 $
// $Workfile: MOS_MapDropEvent.h $
//
// *****************************************************************************

#ifndef __MOS_MapDropEvent_h_
#define __MOS_MapDropEvent_h_

#ifdef __GNUG__
#   pragma interface
#endif

// =============================================================================
/** @class  MOS_MapDropEvent
    @brief  MOS_MapDropEvent
    @par    Using example
    @code
    MOS_MapDropEvent;
    @endcode
*/
// Created: APE 2004-09-02
// =============================================================================
class MOS_MapDropEvent : public QDropEvent
{
    MT_COPYNOTALLOWED( MOS_MapDropEvent );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_MapDropEvent( const QDropEvent& dropEvent, const MT_Vector2D& vMapPos );
    ~MOS_MapDropEvent();
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

#include "MOS_MapDropEvent.inl"

#endif // __MOS_MapDropEvent_h_
