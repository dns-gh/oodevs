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

#include "clients_kernel/Types.h"
#include "clients_kernel/Drawable_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Controller;
    class Displayer_ABC;
    class DotationType;
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
class LogFuneralConsign : public kernel::Drawable_ABC
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
    void Display( kernel::Displayer_ABC& displayer, kernel::Displayer_ABC& itemDisplayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogFuneralConsign( const LogFuneralConsign& );
    LogFuneralConsign& operator=( const LogFuneralConsign& );
    //@}

    //! @name Tools
    //@{
    kernel::Entity_ABC* LogFuneralConsign::FindLogEntity(const sword::ParentEntity& msg);
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    const tools::Resolver_ABC< kernel::Agent_ABC >&   agentResolver_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    const Simulation& simulation_;
    const unsigned int nID_;
    E_HumanRank rank_;
    kernel::Agent_ABC& consumer_;
    kernel::Entity_ABC* handler_;
    kernel::Agent_ABC* convoy_;
    const kernel::DotationType* packagingResource_;
    unsigned int currentStateEndTick_;
    E_LogFuneralHandlingStatus nState_;
    //@}
};

#endif // __LogFuneralConsign_h_
