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
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_AsnException.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Gen_Object )

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( const sword::PlannedWork& msg, const MIL_EntityManager_ABC& entityManager )
    : type_              ( &entityManager.FindObjectType( msg.type() )? msg.type(): "" )
    , identifier_        ( 0u )
    , pObstacleType_     ( msg.type_obstacle() )
    , rDensity_          ( msg.density() )
    , nMinesActivityTime_( msg.activity_time() )
    , pTC2_              ( 0 )
{
    if( type_.empty() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    if( !NET_ASN_Tools::ReadLocation( msg.position(), localisation_ ) )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    if( msg.combat_train().id() != 0 )
    {
        pTC2_ = entityManager.FindAutomate( msg.combat_train().id() );
        if( !pTC2_ )
            throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: LGY 2011-10-25
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( const sword::PlannedWork& msg, const MIL_EntityManager_ABC& entityManager, unsigned int identifier )
    : type_              ( &entityManager.FindObjectType( msg.type() )? msg.type(): "" )
    , identifier_        ( identifier )
    , pObstacleType_     ( msg.type_obstacle() )
    , rDensity_          ( msg.density() )
    , nMinesActivityTime_( msg.activity_time() )
    , pTC2_              ( 0 )
{
    if( type_.empty() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    if( !NET_ASN_Tools::ReadLocation( msg.position(), localisation_ ) )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    if( msg.combat_train().id() != 0 )
    {
        pTC2_ = entityManager.FindAutomate( msg.combat_train().id() );
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
    , identifier_        ( 0u )
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
    , identifier_        ( rhs.identifier_ )
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
    identifier_         = rhs.identifier_;
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
void DEC_Gen_Object::Serialize( sword::PlannedWork& msg ) const
{
    msg.set_type( type_.c_str() );
    msg.set_type_obstacle( pObstacleType_ );
    msg.mutable_combat_train()->set_id( pTC2_ ? pTC2_->GetID() : 0 );
    msg.set_density( static_cast< float >( rDensity_ ) );
    msg.set_activity_time( nMinesActivityTime_ );
    NET_ASN_Tools::WriteLocation( localisation_, *msg.mutable_position() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::load
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void DEC_Gen_Object::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> type_
         >> identifier_
         >> localisation_
         >> pObstacleType_
         >> rDensity_
         >> nMinesActivityTime_
         >> const_cast< MIL_Automate*& >( pTC2_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::save
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void DEC_Gen_Object::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << type_
         << identifier_
         << localisation_
         << pObstacleType_
         << rDensity_
         << nMinesActivityTime_
         << const_cast< MIL_Automate*& >( pTC2_ );
}
