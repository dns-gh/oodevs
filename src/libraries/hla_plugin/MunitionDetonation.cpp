// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "MunitionDetonation.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

namespace hla
{
    template< typename T >
    Serializer& operator<<( Serializer& serializer, const T& parameter )
    {
        parameter.Serialize( serializer );
        return serializer;
    }
    template< typename T >
    Deserializer& operator>>( Deserializer& deserializer, T& /*parameter*/ )
    {
        return deserializer;
    }
}

using namespace plugins::hla;

namespace
{
    template< typename T >
    class PublishOnly : public ::hla::InteractionNotification_ABC< T >
    {
    public:
        virtual void Receive( T& /*interaction*/ )
        {
            throw std::runtime_error( "publish only" );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation constructor
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
MunitionDetonation::MunitionDetonation( Federate_ABC& federate )
    : pNotification_( new PublishOnly< interactions::MunitionDetonation >() )
    , pInteraction_ ( new ::hla::Interaction< interactions::MunitionDetonation >( *pNotification_ ) )
{
    pInteraction_->Register( "ArticulatedPartData"       , ::hla::CreateParameter( &interactions::MunitionDetonation::articulatedPartData ) );
    pInteraction_->Register( "DetonationLocation"        , ::hla::CreateParameter( &interactions::MunitionDetonation::detonationLocation ) );
    pInteraction_->Register( "DetonationResultCode"      , ::hla::CreateParameter( &interactions::MunitionDetonation::detonationResultCode ) );
    pInteraction_->Register( "EventIdentifier"           , ::hla::CreateParameter( &interactions::MunitionDetonation::eventIdentifier ) );
    pInteraction_->Register( "FiringObjectIdentifier"    , ::hla::CreateParameter( &interactions::MunitionDetonation::firingObjectIdentifier ) );
    pInteraction_->Register( "FinalVelocityVector"       , ::hla::CreateParameter( &interactions::MunitionDetonation::finalVelocityVector ) );
    pInteraction_->Register( "FuseType"                  , ::hla::CreateParameter( &interactions::MunitionDetonation::fuseType ) );
    pInteraction_->Register( "MunitionObjectIdentifier"  , ::hla::CreateParameter( &interactions::MunitionDetonation::munitionObjectIdentifier ) );
    pInteraction_->Register( "MunitionType"              , ::hla::CreateParameter( &interactions::MunitionDetonation::munitionType ) );
    pInteraction_->Register( "QuantityFired"             , ::hla::CreateParameter( &interactions::MunitionDetonation::quantityFired ) );
    pInteraction_->Register( "RateOfFire"                , ::hla::CreateParameter( &interactions::MunitionDetonation::rateOfFire ) );
    pInteraction_->Register( "RelativeDetonationLocation", ::hla::CreateParameter( &interactions::MunitionDetonation::relativeDetonationLocation ) );
    pInteraction_->Register( "TargetObjectIdentifier"    , ::hla::CreateParameter( &interactions::MunitionDetonation::targetObjectIdentifier ) );
    pInteraction_->Register( "WarheadType"               , ::hla::CreateParameter( &interactions::MunitionDetonation::targetObjectIdentifier ) );
    federate.Register( ::hla::InteractionIdentifier( "MunitionDetonation" ), *pInteraction_, true, false );
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation destructor
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
MunitionDetonation::~MunitionDetonation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::Send
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void MunitionDetonation::Send( const interactions::MunitionDetonation& interaction )
{
    pInteraction_->Send( interaction );
}
