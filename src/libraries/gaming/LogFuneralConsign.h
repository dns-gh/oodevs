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
#include <boost/function.hpp>

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
                                  const Simulation& simulation_ );
    virtual ~LogFuneralConsign();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::LogFuneralHandlingUpdate& message );
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual bool RefersToAgent( unsigned int id ) const;
    //@}

    //! @name Accessors
    //@{
    E_HumanRank GetRank() const;
    virtual const kernel::Agent_ABC* GetConsumer() const;
    virtual const kernel::Entity_ABC* GetHandler() const;
    const kernel::Agent_ABC* GetConvoy() const;
    const kernel::DotationType* GetPackagingResource() const;
    E_LogFuneralHandlingStatus GetStatus() const;
    virtual QString GetStatusDisplay() const;
    virtual QString GetStatusDisplay( int status ) const;
    virtual QString GetCurrentStartedTime() const;
    //@}

private:
    kernel::Entity_ABC* GetRequestHandler( uint32_t entityId ) const;
    //! @name Copy/Assignment
    //@{
    LogFuneralConsign( const LogFuneralConsign& );
    LogFuneralConsign& operator=( const LogFuneralConsign& );
    //@}

    //! @name Tools
    //@{
    kernel::Entity_ABC* FindLogEntity(const sword::ParentEntity& msg);
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
    E_LogFuneralHandlingStatus nState_;
    //@}
};

#endif // __LogFuneralConsign_h_
