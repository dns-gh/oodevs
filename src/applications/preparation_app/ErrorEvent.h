// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ErrorEvent_h_
#define __ErrorEvent_h_

#include <QtGui/qevent.h>

// =============================================================================
/** @class  ErrorEvent
    @brief  ErrorEvent
*/
// Created: HBD 2010-11-08
// =============================================================================
class ErrorEvent : public QEvent
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ErrorEvent( const QString& reason )
        : QEvent( QEvent::User )
        , reason_( reason )
    {}
    //@}

public:
    //! @name Member data
    //@{
    QString reason_;
    //@}
};

#endif // __ErrorEvent_h_
