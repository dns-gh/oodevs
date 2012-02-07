// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_UnitModelReloader_h
#define plugins_hla_UnitModelReloader_h

#include "ResponseObserver_ABC.h"

namespace sword
{
    class UnitCreation;
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
/** @class  UnitModelReloader
    @brief  UnitModelReloader
*/
// Created: SLI 2011-09-20
// =============================================================================
class UnitModelReloader : private ResponseObserver_ABC< sword::UnitCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitModelReloader( ContextHandler_ABC< sword::UnitCreation >& contextHandler, dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& contextFactory );
    virtual ~UnitModelReloader();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitCreation& message, const std::string& identifier );
    //@}

private:
    //! @name Member data
    //@{
    ContextHandler_ABC< sword::UnitCreation >& contextHandler_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    const ContextFactory_ABC& contextFactory_;
    //@}
};

}
}

#endif // plugins_hla_UnitModelReloader_h

