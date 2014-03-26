// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogSupplyConsign_h_
#define __LogSupplyConsign_h_

#include "LogisticsConsign_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/Types.h"
#include <tools/Resolver.h>
#include <boost/function.hpp>

class SupplyRecipientResourcesRequest;

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controller;
    class Entity_ABC;
    class Formation_ABC;
    class DisplayExtractor_ABC;
    class DotationType;
}

namespace sword
{
    class ParentEntity;
    class LogSupplyHandlingCreation;
    class LogSupplyHandlingUpdate;
    enum LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus;
    class SupplyRecipientResourceRequests;
}

class Simulation;

// =============================================================================
/** @class  LogSupplyConsign
    @brief  LogSupplyConsign
*/
// Created: AGE 2006-02-21
// $$$$ AGE 2006-04-21: factorisations entre types de consignes
// =============================================================================
class LogSupplyConsign : public tools::Resolver< SupplyRecipientResourcesRequest >
                       , public LogisticsConsign_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             LogSupplyConsign( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver,
                               const tools::Resolver_ABC< kernel::Agent_ABC >& agentResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver,
                               const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                               const Simulation& simulation, const sword::LogSupplyHandlingCreation& message );
    virtual ~LogSupplyConsign();
    //@}

    //! @name Operations
    //@{
    bool Update( const sword::LogSupplyHandlingUpdate& message, kernel::Agent_ABC* pionLogConvoying );
    void Update( const sword::SupplyRecipientResourceRequests& message );
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual bool RefersToAgent( unsigned long id ) const;
    virtual bool RefersToAgent( const std::set< unsigned long >& id ) const;
    virtual bool UpdateHistoryState( const sword::LogHistoryEntry& entry, HistoryState& state );
    //@}

    //! @name Accessors
    //@{
    virtual kernel::Agent_ABC* GetConsumer() const;
    virtual kernel::Entity_ABC* GetHandler() const;
    virtual kernel::Agent_ABC* GetConvoy() const;
    const kernel::Entity_ABC* GetProviding() const;
    sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus GetStatus() const;
    virtual QString GetStatusDisplay() const;
    virtual QString GetStatusDisplay( int status ) const;
    virtual QString GetCurrentStartedTime() const;
    virtual E_LogisticChain GetType() const;
    //@}

private:
    virtual kernel::Entity_ABC* GetRequestHandler( uint32_t entityId ) const;

    //! @name Tools
    //@{
    kernel::Entity_ABC* FindLogEntity(const sword::ParentEntity& msg);
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::Automat_ABC >& resolver_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& agentResolver_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    kernel::SafePointer< kernel::Entity_ABC > pLogHandlingEntity_;
    kernel::SafePointer< kernel::Agent_ABC > pPionLogConvoying_;
    kernel::SafePointer< kernel::Entity_ABC > pLogProvidingConvoyResourcesEntity_;
    sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus nState_;
    //@}
};

#endif // __LogSupplyConsign_h_
