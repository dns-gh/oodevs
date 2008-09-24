// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimulationCommands_h_
#define __SimulationCommands_h_

#include "dispatcher/Registrable_ABC.h"
#include "dispatcher/Position.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  SimulationCommands
    @brief  SimulationCommands
*/
// Created: AGE 2008-06-12
// =============================================================================
class SimulationCommands : public dispatcher::Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SimulationCommands( dispatcher::SimulationPublisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
    virtual ~SimulationCommands();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimulationCommands( const SimulationCommands& );            //!< Copy constructor
    SimulationCommands& operator=( const SimulationCommands& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Pause();
    void Resume();
    void ChangeTimeFactor( unsigned int factor );
    void CreateUnit( const dispatcher::Position& position, unsigned int type, unsigned int parent );
    void Engage( unsigned automat, bool engage );
    //@}

private:
    //! @name member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

}
}

#endif // __SimulationCommands_h_
