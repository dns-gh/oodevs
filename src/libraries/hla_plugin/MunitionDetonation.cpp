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
#include "RemoteAgentResolver_ABC.h"
#include "RemoteAgentSubject_ABC.h"
#include "Omt13String.h"
#include "rpr/Coordinates.h"
#include "rpr/EntityType.h"
#include "protocol/Simulation.h"
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>
#include <boost/lexical_cast.hpp>

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
    struct EventIdentifierStruct
    {
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            archive << eventCount;
            issuingObjectIdentifier.Serialize( archive );
        }
        uint16 eventCount;
        Omt13String issuingObjectIdentifier;
    };
}

struct MunitionDetonation::Parameters
{
public:
    uint32 articulatedPartData;
    rpr::WorldLocation detonationLocation;
    int8 detonationResultCode;
    EventIdentifierStruct eventIdentifier;
    Omt13String firingObjectIdentifier;
    rpr::VelocityVector finalVelocityVector;
    int16 fuseType;
    Omt13String munitionObjectIdentifier;
    rpr::EntityType munitionType;
    uint16 quantityFired;
    uint16 rateOfFire;
    rpr::VelocityVector relativeDetonationLocation; // RelativePositionStruct
    Omt13String targetObjectIdentifier;
    int16 warheadType;
};

// -----------------------------------------------------------------------------
// Name: MunitionDetonation constructor
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
MunitionDetonation::MunitionDetonation( Federate_ABC& federate, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                        const RemoteAgentResolver_ABC& resolver, RemoteAgentSubject_ABC& remoteAgentSubject,
                                        const std::string& federateName )
    : resolver_          ( resolver )
    , remoteAgentSubject_( remoteAgentSubject )
    , federateName_      ( federateName )
    , pNotification_     ( new PublishOnly< Parameters >() )
    , pInteraction_      ( new ::hla::Interaction< Parameters >( *pNotification_ ) )
{
    CONNECT( controller, *this, start_unit_fire );
    CONNECT( controller, *this, stop_unit_fire );
    remoteAgentSubject_.Register( *this );
    pInteraction_->Register( "ArticulatedPartData"       , ::hla::CreateParameter( &Parameters::articulatedPartData ) );
    pInteraction_->Register( "DetonationLocation"        , ::hla::CreateParameter( &Parameters::detonationLocation ) );
    pInteraction_->Register( "DetonationResultCode"      , ::hla::CreateParameter( &Parameters::detonationResultCode ) );
    pInteraction_->Register( "EventIdentifier"           , ::hla::CreateParameter( &Parameters::eventIdentifier ) );
    pInteraction_->Register( "FiringObjectIdentifier"    , ::hla::CreateParameter( &Parameters::firingObjectIdentifier ) );
    pInteraction_->Register( "FinalVelocityVector"       , ::hla::CreateParameter( &Parameters::finalVelocityVector ) );
    pInteraction_->Register( "FuseType"                  , ::hla::CreateParameter( &Parameters::fuseType ) );
    pInteraction_->Register( "MunitionObjectIdentifier"  , ::hla::CreateParameter( &Parameters::munitionObjectIdentifier ) );
    pInteraction_->Register( "MunitionType"              , ::hla::CreateParameter( &Parameters::munitionType ) );
    pInteraction_->Register( "QuantityFired"             , ::hla::CreateParameter( &Parameters::quantityFired ) );
    pInteraction_->Register( "RateOfFire"                , ::hla::CreateParameter( &Parameters::rateOfFire ) );
    pInteraction_->Register( "RelativeDetonationLocation", ::hla::CreateParameter( &Parameters::relativeDetonationLocation ) );
    pInteraction_->Register( "TargetObjectIdentifier"    , ::hla::CreateParameter( &Parameters::targetObjectIdentifier ) );
    pInteraction_->Register( "WarheadType"               , ::hla::CreateParameter( &Parameters::targetObjectIdentifier ) );
    federate.Register( ::hla::InteractionIdentifier( "MunitionDetonation" ), *pInteraction_, true, false );
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation destructor
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
MunitionDetonation::~MunitionDetonation()
{
    remoteAgentSubject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::Notify
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
void MunitionDetonation::Notify( const sword::StartUnitFire& message, int /*context*/ )
{
    fires_[ message.fire().id() ] = message;
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::Notify
// Created: SLI 2011-06-27
// -----------------------------------------------------------------------------
void MunitionDetonation::Notify( const sword::StopUnitFire& message, int /*context*/ )
{
    const unsigned int fireIdentifier = message.fire().id();
    if( fires_.find( fireIdentifier ) == fires_.end() )
        return;
    const sword::StartUnitFire& startMessage = fires_[ fireIdentifier ];
    const bool isDirect = startMessage.type() == sword::StartUnitFire::direct;
    const std::string targetTdentifier = resolver_.Resolve( startMessage.target().unit().id() );
    if( isDirect && targetTdentifier.empty() )
        return;
    Parameters parameters;
    parameters.articulatedPartData = 0u;  // empty array
    parameters.detonationLocation = isDirect ? positions_[ targetTdentifier ] : rpr::WorldLocation( startMessage.target().position().latitude(), startMessage.target().position().longitude(), 0. );
    parameters.detonationResultCode = isDirect ? 1 : 3; // EntityImpact or GroundImpact
    parameters.eventIdentifier.eventCount = static_cast< uint16 >( fireIdentifier );
    parameters.eventIdentifier.issuingObjectIdentifier = Omt13String( federateName_ );
    parameters.firingObjectIdentifier = Omt13String( boost::lexical_cast< std::string >( startMessage.firing_unit().id() ) );
    pInteraction_->Send( parameters );
    fires_.erase( fireIdentifier );
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::Created
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void MunitionDetonation::Created( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::Destroyed
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void MunitionDetonation::Destroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::Moved
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void MunitionDetonation::Moved( const std::string& identifier, double latitude, double longitude )
{
    positions_[ identifier ] = rpr::WorldLocation( latitude, longitude, 0. );
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::SideChanged
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void MunitionDetonation::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::NameChanged
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void MunitionDetonation::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::TypeChanged
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void MunitionDetonation::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}
