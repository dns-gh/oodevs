// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __PopulationConditions_h_
#define __PopulationConditions_h_

#include "dispatcher/Registrable_ABC.h"
#include "dispatcher/Position.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Agent;
}

namespace plugins
{
namespace script
{
    class Condition_ABC;

// =============================================================================
/** @class  PopulationConditions
    @brief  PopulationConditions
*/
// Created: SBO 2008-11-18
// =============================================================================
class PopulationConditions : public dispatcher::Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationConditions( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~PopulationConditions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationConditions( const PopulationConditions& );            //!< Copy constructor
    PopulationConditions& operator=( const PopulationConditions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    boost::shared_ptr< Condition_ABC > PopulationEnters( const dispatcher::Zone& zone );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

}
}

#endif // __PopulationConditions_h_
