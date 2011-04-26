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

#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

class IndicatorExportDialog;
class IndicatorRequest;
class IndicatorPlot;
class Publisher_ABC;
class Services;
class Simulation;

// =============================================================================
/** @class  IndicatorPlotFactory
    @brief  IndicatorPlotFactory
*/
// Created: SBO 2009-04-30
// =============================================================================
class IndicatorPlotFactory : public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< Services >
                           , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             IndicatorPlotFactory( QMainWindow* mainWindow, kernel::Controllers& controllers, Publisher_ABC& publisher, IndicatorExportDialog& exportDialog, const Simulation& simulation );
    virtual ~IndicatorPlotFactory();
    //@}

    //! @name Operations
    //@{
    IndicatorPlot* CreatePlot( const IndicatorRequest& request );
    //@}

private:
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
    IndicatorExportDialog& exportDialog_;
    bool hasReplay_;
    const Simulation& simulation_;
    //@}
};

#endif // __IndicatorPlotFactory_h_
