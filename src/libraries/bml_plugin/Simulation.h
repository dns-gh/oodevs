// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Simulation_h_
#define __Simulation_h_

#include "protocol/protocol.h"

namespace plugins
{
namespace bml
{

// =============================================================================
/** @class  Simulation
    @brief  Simulation
*/
// Created: SBO 2008-06-09
// =============================================================================
class Simulation
{
public:
    //! @name Constructors/Destructor
    //@{
             Simulation();
    virtual ~Simulation();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::ControlEndTick& message );
    //@}

    //! @name Accessors
    //@{
    bool MustReportPosition( unsigned long& currentTick ) const;
    bool MustReportStatus( unsigned long& currentTick ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Simulation( const Simulation& );            //!< Copy constructor
    Simulation& operator=( const Simulation& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned long currentTick_;
    unsigned int tickDuration_;
    //@}
};

}
}

#endif // __Simulation_h_
