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
#include "rpr/Coordinates.h"
#include "rpr/EntityType.h"
#include "protocol/Simulation.h"
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

struct MunitionDetonation::Parameters
{
public:
    uint32 articulatedPartData;
    rpr::WorldLocation detonationLocation;
    int8 detonationResultCode;
    uint16 eventCount;
    char issuingObjectIdentifier[13];
    char firingObjectIdentifier[13];
    rpr::VelocityVector finalVelocityVector;
    int16 fuseType;
    char munitionObjectIdentifier[13];
    rpr::EntityType munitionType;
    uint16 quantityFired;
    uint16 rateOfFire;
    rpr::VelocityVector relativeDetonationLocation; // RelativePositionStruct
    char targetObjectIdentifier[13];
    int16 warheadType;
};

// -----------------------------------------------------------------------------
// Name: MunitionDetonation constructor
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
MunitionDetonation::MunitionDetonation( Federate_ABC& federate, tools::MessageController_ABC< sword::SimToClient_Content >& controller )
    : pNotification_( new PublishOnly< Parameters >() )
    , pInteraction_ ( new ::hla::Interaction< Parameters >( *pNotification_ ) )
{
    CONNECT( controller, *this, start_unit_fire );
    CONNECT( controller, *this, start_fire_effect );
    pInteraction_->Register( "ArticulatedPartData"       , ::hla::CreateParameter( &Parameters::articulatedPartData ) );
    pInteraction_->Register( "DetonationLocation"        , ::hla::CreateParameter( &Parameters::detonationLocation ) );
    pInteraction_->Register( "DetonationResultCode"      , ::hla::CreateParameter( &Parameters::detonationResultCode ) );
    pInteraction_->Register( "EventIdentifier"           , ::hla::CreateParameter( &Parameters::eventCount ) );
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::Notify
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
void MunitionDetonation::Notify( const sword::StartUnitFire& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MunitionDetonation::Notify
// Created: SLI 2011-06-27
// -----------------------------------------------------------------------------
void MunitionDetonation::Notify( const sword::StartFireEffect& message )
{
    Parameters parameters;
    parameters.articulatedPartData = 0u;
    if( message.location().type() != sword::Location_Geometry_none && message.location().coordinates().elem_size() > 0 )
    {
        const sword::CoordLatLong& center = message.location().coordinates().elem( 0 );
        parameters.detonationLocation = rpr::WorldLocation( center.latitude(), center.longitude(), 0. );
    }
    pInteraction_->Send( parameters );
}
