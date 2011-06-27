// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MunitionDetonation_h
#define plugins_hla_MunitionDetonation_h

#include "tools/MessageObserver.h"

namespace sword
{
    class SimToClient_Content;
    class StartUnitFire;
}

namespace hla
{
    template< typename T > class Interaction;
    template< typename T > class InteractionNotification_ABC;
}

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace plugins
{
namespace hla
{
    class Federate_ABC;

// =============================================================================
/** @class  MunitionDetonation
    @brief  Munition detonation
*/
// Created: SLI 2011-06-24
// =============================================================================
class MunitionDetonation : private tools::MessageObserver< sword::StartUnitFire >
{
public:
    //! @name Constructors/Destructor
    //@{
             MunitionDetonation( Federate_ABC& federate, tools::MessageController_ABC< sword::SimToClient_Content >& controller );
    virtual ~MunitionDetonation();
    //@}

private:
    //! @name Messages
    //@{
    virtual void Notify( const sword::StartUnitFire& message );
    //@}

private:
    //! @name Types
    //@{
    struct Parameters;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::InteractionNotification_ABC< Parameters > > pNotification_;
    std::auto_ptr< ::hla::Interaction< Parameters > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_MunitionDetonation_h
