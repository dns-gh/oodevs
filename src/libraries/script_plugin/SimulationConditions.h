// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimulationConditions_h_
#define __SimulationConditions_h_

#include "dispatcher/Registrable_ABC.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controller;
}

namespace plugins
{
namespace script
{
    class Condition_ABC;
    class Controller;

// =============================================================================
/** @class  SimulationConditions
    @brief  Simulation conditions
*/
// Created: AGE 2008-06-12
// =============================================================================
class SimulationConditions : public dispatcher::Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SimulationConditions( kernel::Controller& controller );
    virtual ~SimulationConditions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::brain::Brain& brain );
    //@}

private:
    //! @name Helpers
    //@{
    boost::shared_ptr< Condition_ABC > TickEnded();
    boost::shared_ptr< Condition_ABC > ClientConnected();
    boost::shared_ptr< Condition_ABC > ClientLeft();
	boost::shared_ptr< Condition_ABC > AtDateTime( const std::string& triggerTime );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimulationConditions( const SimulationConditions& );
    SimulationConditions& operator=( const SimulationConditions& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

}
}

#endif // __SimulationConditions_h_
