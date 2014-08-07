// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogFuneralConsign_h_
#define __LogFuneralConsign_h_

#include "LogisticsConsign_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/Types.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Controller;
    class DotationType;
    class DisplayExtractor_ABC;
}

namespace sword
{
    class LogFuneralHandlingCreation;
    class LogFuneralHandlingUpdate;
    class ParentEntity;
    enum LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus;
}

class Simulation;

// =============================================================================
/** @class  LogFuneralConsign
    @brief  LogFuneralConsign
*/
// Created: AGE 2006-02-28
// =============================================================================
class LogFuneralConsign : public LogisticsConsign_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             LogFuneralConsign( kernel::Controller& controller, const sword::LogFuneralHandlingCreation& message,
                                  const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                                  const tools::Resolver_ABC< kernel::Agent_ABC >& agentResolver,
                                  const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver,
                                  const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                                  const Simulation& simulation_, kernel::Agent_ABC& consumer );
    virtual ~LogFuneralConsign();
    //@}

    //! @name Operations
    //@{
    bool Update( const sword::LogFuneralHandlingUpdate& message, kernel::Entity_ABC* handler, kernel::Agent_ABC* convoy );
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual bool RefersToAgent( unsigned long id ) const;
    virtual bool RefersToAgent( const std::set< unsigned long >& id ) const;
    //@}

    //! @name Accessors
    //@{
    E_HumanRank GetRank() const;
    virtual kernel::Agent_ABC* GetConsumer() const;
    virtual kernel::Entity_ABC* GetHandler() const;
    kernel::Agent_ABC* GetConvoy() const;
    const kernel::DotationType* GetPackagingResource() const;
    sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus GetStatus() const;
    virtual QString GetStatusDisplay() const;
    virtual QString GetStatusDisplay( int status ) const;
    virtual QString GetCurrentStartedTime() const;
    virtual E_LogisticChain GetType() const;
    //@}

private:
    //! @name Helpers
    //@{
    kernel::Entity_ABC* GetRequestHandler( uint32_t entityId ) const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    const tools::Resolver_ABC< kernel::Agent_ABC >&   agentResolver_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    E_HumanRank rank_;
    kernel::Agent_ABC& consumer_;
    kernel::SafePointer< kernel::Entity_ABC > handler_;
    kernel::SafePointer< kernel::Agent_ABC > convoy_;
    const kernel::DotationType* packagingResource_;
    sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus nState_;
    //@}
};

#endif // __LogFuneralConsign_h_
