// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimulationPublisher_ABC_h_
#define __SimulationPublisher_ABC_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class DispatcherToSim;
}

namespace sword
{
    class ClientToSim;
}

namespace dispatcher
{
// =============================================================================
/** @class  SimulationPublisher_ABC
    @brief  SimulationPublisher_ABC
*/
// Created: AGE 2007-08-24
// =============================================================================
class SimulationPublisher_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SimulationPublisher_ABC() {}
    virtual ~SimulationPublisher_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( const sword::ClientToSim& msg ) = 0;
    virtual void Send( const sword::DispatcherToSim& msg ) = 0;
    //@}
};

}

#endif // __SimulationPublisher_ABC_h_
