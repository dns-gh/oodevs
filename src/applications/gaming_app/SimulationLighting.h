// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimulationLighting_h_
#define __SimulationLighting_h_

#include "clients_gui/LightingProxy.h"
#include "clients_kernel/ModelLoaded.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
}
class Simulation;

// =============================================================================
/** @class  SimulationLighting
    @brief  SimulationLighting
*/
// Created: AGE 2007-10-15
// =============================================================================
class SimulationLighting : public gui::LightingProxy
                         , public tools::Observer_ABC
                         , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                         , public tools::ElementObserver_ABC< Simulation >
{
public:
    //! @name Constructors/Destructor
    //@{
             SimulationLighting( kernel::Controllers& controller, QObject* parent );
    virtual ~SimulationLighting();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimulationLighting( const SimulationLighting& );            //!< Copy constructor
    SimulationLighting& operator=( const SimulationLighting& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simu );
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controller_;
    //@}
};

#endif // __SimulationLighting_h_
