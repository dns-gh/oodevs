// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogMaintenanceConsign_h_
#define __LogMaintenanceConsign_h_

#include "clients_kernel/Types.h"
#include "game_asn/Simulation.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class Displayer_ABC;
    class ComponentType;
    class BreakdownType;
}

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogMaintenanceConsign : public kernel::Drawable_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             LogMaintenanceConsign( kernel::Controller& controller, const ASN1T_MsgLogMaintenanceHandlingCreation& asn,
                                    const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, const kernel::Resolver_ABC< kernel::ComponentType >& componentResolver,
                                    const kernel::Resolver_ABC< kernel::BreakdownType >& breakdownResolver );
    virtual ~LogMaintenanceConsign();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer, kernel::Displayer_ABC& itemDisplayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name Network events
    //@{
    void Update( const ASN1T_MsgLogMaintenanceHandlingUpdate& asn );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogMaintenanceConsign( const LogMaintenanceConsign& );
    LogMaintenanceConsign& operator=( const LogMaintenanceConsign& );
    //@}

private:
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    uint    nID_;
    kernel::Agent_ABC&  pion_;
    kernel::Agent_ABC*  pPionLogHandling_;
    const kernel::ComponentType* equipmentType_;
    const kernel::BreakdownType* breakdownType_;
    bool    diagnosed_;
    E_LogMaintenanceHandlingStatus nState_;
};

#endif // __LogMaintenanceConsign_h_
