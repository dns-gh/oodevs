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

#include "LogisticsConsign_ABC.h"
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

// =============================================================================
/** @class  LogMaintenanceConsign
    @brief  LogMaintenanceConsign
*/
// Created: AGE 2006-02-28
// =============================================================================
class LogMaintenanceConsign : public LogisticsConsign_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             LogMaintenanceConsign( kernel::Controller& controller, const sword::LogMaintenanceHandlingCreation& message,
                                    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, const tools::Resolver_ABC< kernel::ComponentType >& componentResolver,
                                    const tools::Resolver_ABC< kernel::BreakdownType >& breakdownResolver, const Simulation& simulation,
                                    kernel::Agent_ABC& consumer );
    virtual ~LogMaintenanceConsign();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::LogMaintenanceHandlingUpdate& message, kernel::Entity_ABC* handler );
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual bool RefersToAgent( unsigned int id ) const;
    //@}

    //! @name Accessors
    //@{
    virtual kernel::Agent_ABC* GetConsumer() const;
    virtual kernel::Entity_ABC* GetHandler() const;
    virtual kernel::Agent_ABC* GetConvoy() const;
    const kernel::ComponentType* GetEquipment() const;
    const kernel::BreakdownType* GetBreakdown() const;
    bool IsDiagnosed() const;
    E_LogMaintenanceHandlingStatus GetStatus() const;
    virtual QString GetStatusDisplay() const;
    virtual QString GetStatusDisplay( int status ) const;
    virtual QString GetCurrentStartedTime() const;
    //@}

private:
    kernel::Entity_ABC* GetRequestHandler( uint32_t entityId ) const;
    //! @name Copy/Assignment
    //@{
    LogMaintenanceConsign( const LogMaintenanceConsign& );
    LogMaintenanceConsign& operator=( const LogMaintenanceConsign& );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    kernel::Agent_ABC& consumer_;
    kernel::Entity_ABC* pPionLogHandling_;
    const kernel::ComponentType* equipmentType_;
    const kernel::BreakdownType* breakdownType_;
    bool diagnosed_;
    E_LogMaintenanceHandlingStatus nState_;
    //@}
};

#endif // __LogMaintenanceConsign_h_
