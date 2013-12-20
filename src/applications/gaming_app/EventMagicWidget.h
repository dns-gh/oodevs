// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventMagicWidget_h_
#define __EventMagicWidget_h_

#include "EventWidget_ABC.h"

// =============================================================================
/** @class  EventMagicWidget
    @brief  EventMagicWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventMagicWidget : public EventDefaultWidget_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventMagicWidget( gui::EventPresenter& presenter );
    virtual ~EventMagicWidget();
    //@}
};

#endif // __EventMagicWidget_h_
