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

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Types.h"
#include <tools/Resolver_ABC.h>
#include <boost/function.hpp>

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class ComponentType;
    class BreakdownType;
    class DisplayExtractor_ABC;
}

namespace sword
{
    class LogMaintenanceHandlingCreation;
    class LogMaintenanceHandlingUpdate;
}

class Simulation;
class LogConsignDisplayer_ABC;

// =============================================================================
/** @class  LogMaintenanceConsign
    @brief  LogMaintenanceConsign
*/
// Created: AGE 2006-02-28
// =============================================================================
class LogMaintenanceConsign : public gui::Drawable_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             LogMaintenanceConsign( kernel::Controller& controller, const sword::LogMaintenanceHandlingCreation& message,
                                    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, const tools::Resolver_ABC< kernel::ComponentType >& componentResolver,
                                    const tools::Resolver_ABC< kernel::BreakdownType >& breakdownResolver, const Simulation& simulation );
    virtual ~LogMaintenanceConsign();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::LogMaintenanceHandlingUpdate& message );
    void Display( LogConsignDisplayer_ABC& displayer, kernel::DisplayExtractor_ABC& displayExtractor ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    unsigned int GetId() const;
    bool RefersToAgent( unsigned int id ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogMaintenanceConsign( const LogMaintenanceConsign& );
    LogMaintenanceConsign& operator=( const LogMaintenanceConsign& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    const Simulation& simulation_;
    unsigned int nID_;
    kernel::Agent_ABC& consumer_;
    kernel::Agent_ABC* pPionLogHandling_;
    const kernel::ComponentType* equipmentType_;
    const kernel::BreakdownType* breakdownType_;
    bool diagnosed_;
    E_LogMaintenanceHandlingStatus nState_;
    unsigned int currentStateEndTick_;
    //@}
};

#endif // __LogMaintenanceConsign_h_
