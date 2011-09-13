// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AutomatDisengager_h
#define plugins_hla_AutomatDisengager_h

#include "ResponseObserver_ABC.h"

namespace sword
{
    class AutomatCreation;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace hla
{
    template< typename ResponseMessage > class ContextHandler_ABC;
    class ContextFactory_ABC;

// =============================================================================
/** @class  AutomatDisengager
    @brief  Automat disengager
*/
// Created: SLI 2011-09-13
// =============================================================================
class AutomatDisengager : private ResponseObserver_ABC< sword::AutomatCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatDisengager( ContextHandler_ABC< sword::AutomatCreation >& contextHandler, dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& contextFactory );
    virtual ~AutomatDisengager();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::AutomatCreation& message, const std::string& identifier );
    //@}

private:
    //! @name Member data
    //@{
    ContextHandler_ABC< sword::AutomatCreation >& contextHandler_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    const ContextFactory_ABC& contextFactory_;
    //@}
};

}
}

#endif // plugins_hla_AutomatDisengager_h
