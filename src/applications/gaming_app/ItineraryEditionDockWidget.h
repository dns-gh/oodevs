// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ItineraryEditionDockWidget_h_
#define __ItineraryEditionDockWidget_h_

#include "clients_gui/RichDockWidget.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Filter_ABC;
}

// =============================================================================
/** @class  ItineraryEditionDockWidget
    @brief  ItineraryEditionDockWidget
*/
// Created: SLI 2014-04-09
// =============================================================================
class ItineraryEditionDockWidget : public gui::RichDockWidget
                                 , public tools::ElementObserver_ABC< kernel::Filter_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ItineraryEditionDockWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ItineraryEditionDockWidget();
    //@}

signals:
    //! @name signals
    //@{
    void ItineraryAccepted();
    void ItineraryRejected();
    //@}

private:
    virtual void showEvent( QShowEvent* event );
};

#endif // __ItineraryEditionDockWidget_h_
