// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventReportWidget_h_
#define __EventReportWidget_h_

#include "EventWidget_ABC.h"

// =============================================================================
/** @class  EventReportWidget
    @brief  EventReportWidget
*/
// Created: ABR 2013-05-29
// =============================================================================
class EventReportWidget : public EventWidget_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EventReportWidget();
    virtual ~EventReportWidget();
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Fill( const kernel::Event& event );
    virtual void Commit( timeline::Event& event );
    //@}
};

#endif // __EventReportWidget_h_
