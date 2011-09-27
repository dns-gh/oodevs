// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_IndirectFireReceiver_h
#define plugins_hla_IndirectFireReceiver_h

#include <hla/InteractionNotification_ABC.h>

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct MunitionDetonation;
}
    class ContextFactory_ABC;
    class MunitionTypeResolver_ABC;

// =============================================================================
/** @class  IndirectFireReceiver
    @brief  Indirect fire receiver
*/
// Created: SLI 2011-09-27
// =============================================================================
class IndirectFireReceiver : public ::hla::InteractionNotification_ABC< interactions::MunitionDetonation >
{
public:
    //! @name Constructors/Destructor
    //@{
             IndirectFireReceiver( dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& factory, const MunitionTypeResolver_ABC& resolver );
    virtual ~IndirectFireReceiver();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::MunitionDetonation& interaction );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    const ContextFactory_ABC& factory_;
    const MunitionTypeResolver_ABC& resolver_;
    //@}
};

}
}

#endif // plugins_hla_IndirectFireReceiver_h
