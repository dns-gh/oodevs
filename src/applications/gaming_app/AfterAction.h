// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterAction_h_
#define __AfterAction_h_

#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
    class ParametersLayer;
}

class AfterActionModel;
class AfterActionRequest;
class IndicatorPlotFactory;
class Services;
class StaticModel;

// =============================================================================
/** @class  AfterAction
    @brief  AfterAction
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterAction : public QObject
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< Services >
                  , public tools::ElementObserver_ABC< AfterActionRequest >
                  , boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AfterAction( QMainWindow* window, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, AfterActionModel& model,
                          gui::ParametersLayer& layer, const StaticModel& staticModel, IndicatorPlotFactory& plotFactory );
    virtual ~AfterAction();
    //@}

    //! @name Operations
    //@{
    void SetStartup();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateAfterActionDock( QMainWindow* window, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, AfterActionModel& model, 
                                gui::ParametersLayer& layer, const StaticModel& staticModel, IndicatorPlotFactory& plotFactory );
    virtual void NotifyUpdated( const Services& services );
    virtual void NotifyCreated( const AfterActionRequest& );
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow* window_;
    QDockWidget* aarDock_;
    QTabWidget* functionsTab_;
    bool aar_;
    bool firstUpdate_;
    //@}
};

#endif // __AfterAction_h_
