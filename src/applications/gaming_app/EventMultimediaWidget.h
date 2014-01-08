// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventMultimediaWidget_h_
#define __EventMultimediaWidget_h_

#include "EventWidget_ABC.h"

// =============================================================================
/** @class  EventMultimediaWidget
    @brief  EventMultimediaWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventMultimediaWidget : public EventDefaultWidget_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventMultimediaWidget( gui::EventPresenter& presenter );
    virtual ~EventMultimediaWidget();
    //@}
};

#endif // __EventMultimediaWidget_h_
