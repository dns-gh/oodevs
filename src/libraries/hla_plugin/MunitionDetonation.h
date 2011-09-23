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

#include "InteractionSender_ABC.h"

namespace hla
{
    template< typename T > class Interaction;
    template< typename T > class InteractionNotification_ABC;
}

namespace plugins
{
namespace hla
{
    class Federate_ABC;

namespace interactions
{
    struct MunitionDetonation;
}

// =============================================================================
/** @class  MunitionDetonation
    @brief  Munition detonation
*/
// Created: SLI 2011-06-24
// =============================================================================
class MunitionDetonation : public InteractionSender_ABC< interactions::MunitionDetonation >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MunitionDetonation( Federate_ABC& federate );
    virtual ~MunitionDetonation();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::MunitionDetonation& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::InteractionNotification_ABC< interactions::MunitionDetonation > > pNotification_;
    std::auto_ptr< ::hla::Interaction< interactions::MunitionDetonation > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_MunitionDetonation_h
