// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorPlotFactory_h_
#define __IndicatorPlotFactory_h_

#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class IndicatorRequest;
class IndicatorPlot;
class Publisher_ABC;
class Services;

// =============================================================================
/** @class  IndicatorPlotFactory
    @brief  IndicatorPlotFactory
*/
// Created: SBO 2009-04-30
// =============================================================================
class IndicatorPlotFactory : public kernel::Observer_ABC
                           , public kernel::ElementObserver_ABC< Services >
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorPlotFactory( QMainWindow* mainWindow, kernel::Controllers& controllers, Publisher_ABC& publisher );
    virtual ~IndicatorPlotFactory();
    //@}

    //! @name Operations
    //@{
    IndicatorPlot* CreatePlot( const IndicatorRequest& request );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorPlotFactory( const IndicatorPlotFactory& );            //!< Copy constructor
    IndicatorPlotFactory& operator=( const IndicatorPlotFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Services& services );
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow* mainWindow_;
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    bool hasReplay_;
    //@}
};

#endif // __IndicatorPlotFactory_h_
