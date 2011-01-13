// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Gen_Object.h"
#include "MIL_AgentServer.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_AsnException.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( const sword::PlannedWork& asn, const MIL_EntityManager_ABC& entityManager )
    : type_              ( &entityManager.FindObjectType( asn.type() )? asn.type(): "" )
    , pObstacleType_     ( asn.type_obstacle() )
    , rDensity_          ( asn.densite() )
    , nMinesActivityTime_( asn.activity_time() )
    , pTC2_              ( 0 )
{
    if( type_.empty() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    if( !NET_ASN_Tools::ReadLocation( asn.position(), localisation_ ) )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    if( asn.tc2().id() != 0 )
    {
        pTC2_ = entityManager.FindAutomate( asn.tc2().id() );
        if( !pTC2_ )
            throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: MGD 2010-06-16
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( std::string type, boost::shared_ptr< TER_Localisation > location, bool preliminary )
    : type_              ( type )
    , localisation_      ( *location )
    , pObstacleType_     ( preliminary ? sword::ObstacleType_DemolitionTargetType_preliminary : sword::ObstacleType_DemolitionTargetType_reserved )
    , rDensity_          ( 0 )
    , nMinesActivityTime_( 0 )
    , pTC2_              ( 0 )
{
    if( type_.empty() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( const DEC_Gen_Object& rhs )
    : type_              ( rhs.type_ )
    , localisation_      ( rhs.localisation_ )
    , pObstacleType_     ( rhs.pObstacleType_ )
    , rDensity_          ( rhs.rDensity_ )
    , nMinesActivityTime_( rhs.nMinesActivityTime_ )
    , pTC2_              ( rhs.pTC2_ )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DEC_Gen_Object destructor
// Created: AGN 03-08-27
//-----------------------------------------------------------------------------
DEC_Gen_Object::~DEC_Gen_Object()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::operator=
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
DEC_Gen_Object& DEC_Gen_Object::operator=( const DEC_Gen_Object& rhs )
{
    type_               = rhs.type_;
    localisation_       = rhs.localisation_;
    pObstacleType_      = rhs.pObstacleType_;
    rDensity_           = rhs.rDensity_;
    nMinesActivityTime_ = rhs.nMinesActivityTime_;
    pTC2_               = rhs.pTC2_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::Serialize
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
void DEC_Gen_Object::Serialize( sword::PlannedWork& asn ) const
{
    asn.set_type( type_.c_str() );
    asn.set_type_obstacle( pObstacleType_ );
    asn.mutable_tc2()->set_id( pTC2_ ? pTC2_->GetID() : 0 );
    asn.set_densite( static_cast< float >( rDensity_ ) );
    asn.set_activity_time( nMinesActivityTime_ );
    NET_ASN_Tools::WriteLocation( localisation_, *asn.mutable_position() );
}
