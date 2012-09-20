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
#include "Entities/MIL_EntityManager_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/CapacityRetriever.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_AsnException.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Gen_Object )

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object()
    : identifier_( 0 )
    , pObstacleType_( sword::ObstacleType_DemolitionTargetType_preliminary )
    , rDensity_( 0 )
    , nMinesActivityTime_( 0 )
    , nActivationTime_( 0 )
    , pTC2_( 0 )
    , altitudeModifier_( 0 )
    , timeLimit_( 0 )
    , lodging_( 0 )
    , mining_( false )
{
    // NOTHING
}

namespace
{
    void SetLocationSize( const ObjectTypeResolver_ABC& resolver, const std::string& type, TER_Localisation& localisation )
    {
        if( !type.empty() && localisation.GetType() == TER_Localisation::ePoint )
        {
            const MIL_ObjectType_ABC& objectType = resolver.FindType( type );
            localisation.Reset( localisation, objectType.GetPointSize() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( const sword::PlannedWork& msg, const MIL_EntityManager_ABC& entityManager,
                                const ObjectTypeResolver_ABC& resolver )
    : type_              ( &resolver.FindType( msg.type() )? msg.type(): "" )
    , identifier_        ( 0u )
    , pObstacleType_     ( msg.has_type_obstacle() ? msg.type_obstacle() : sword::ObstacleType_DemolitionTargetType_preliminary )
    , rDensity_          ( msg.has_density() ? msg.density() : 0. )
    , nMinesActivityTime_( msg.has_activity_time() ? msg.activity_time() : 0 )
    , nActivationTime_   ( msg.has_activation_time() ? msg.activation_time() : 0 )
    , name_              ( msg.has_name() ? msg.name() : std::string() )
    , pTC2_              ( 0 )
    , altitudeModifier_  ( msg.has_altitude_modifier() ? msg.altitude_modifier() : 0 )
    , timeLimit_         ( msg.has_time_limit() ? msg.time_limit() : 0 )
    , lodging_           ( msg.has_lodging() ? msg.lodging() : 0 )
    , mining_            ( msg.has_mining() ? msg.mining() : false )
{
    if( type_.empty() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    if( !NET_ASN_Tools::ReadLocation( msg.position(), localisation_ ) )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    SetLocationSize( resolver, type_, localisation_ );
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
DEC_Gen_Object::DEC_Gen_Object( const sword::PlannedWork& msg, const MIL_EntityManager_ABC& entityManager, unsigned int identifier,
                                const ObjectTypeResolver_ABC& resolver )
    : type_              ( &resolver.FindType( msg.type() )? msg.type(): "" )
    , identifier_        ( identifier )
    , pObstacleType_     ( msg.has_type_obstacle() ? msg.type_obstacle() : sword::ObstacleType_DemolitionTargetType_preliminary )
    , rDensity_          ( msg.has_density() ? msg.density() : 0. )
    , nMinesActivityTime_( msg.has_activity_time() ? msg.activity_time() : 0 )
    , nActivationTime_   ( msg.has_activation_time() ? msg.activation_time() : 0 )
    , name_              ( msg.has_name() ? msg.name() : std::string() )
    , pTC2_              ( 0 )
    , altitudeModifier_  ( msg.has_altitude_modifier() ? msg.altitude_modifier() : 0 )
    , timeLimit_         ( msg.has_time_limit() ? msg.time_limit() : 0 )
    , lodging_           ( msg.has_lodging() ? msg.lodging() : 0 )
    , mining_            ( msg.has_mining() ? msg.mining() : false )
{
    if( type_.empty() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    if( !NET_ASN_Tools::ReadLocation( msg.position(), localisation_ ) )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    SetLocationSize( resolver, type_, localisation_ );
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
DEC_Gen_Object::DEC_Gen_Object( std::string type, TER_Localisation* location, bool preliminary )
    : type_              ( type )
    , identifier_        ( 0u )
    , localisation_      ( *location )
    , pObstacleType_     ( preliminary ? sword::ObstacleType_DemolitionTargetType_preliminary : sword::ObstacleType_DemolitionTargetType_reserved )
    , rDensity_          ( 0 )
    , nMinesActivityTime_( 0 )
    , nActivationTime_   ( 0 )
    , pTC2_              ( 0 )
    , altitudeModifier_  ( 0 )
    , timeLimit_         ( 0 )
    , lodging_           ( 0 )
    , mining_            ( false )
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
    , nActivationTime_   ( rhs.nActivationTime_ )
    , name_              ( rhs.name_ )
    , pTC2_              ( rhs.pTC2_ )
    , altitudeModifier_  ( rhs.altitudeModifier_ )
    , timeLimit_         ( rhs.timeLimit_ )
    , lodging_           ( 0 )
    , mining_            ( rhs.mining_ )
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
    nActivationTime_    = rhs.nActivationTime_;
    name_               = rhs.name_;
    pTC2_               = rhs.pTC2_;
    altitudeModifier_   = rhs.altitudeModifier_;
    timeLimit_          = rhs.timeLimit_;
    lodging_            = rhs.lodging_;
    mining_             = rhs.mining_;
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
    msg.set_activation_time( nActivationTime_ );
    if( !name_.empty() )
        msg.set_name( name_ );
    NET_ASN_Tools::WriteLocation( localisation_, *msg.mutable_position() );
    msg.set_altitude_modifier( altitudeModifier_ );
    msg.set_time_limit( timeLimit_ );
    msg.set_mining( mining_ );
    msg.set_lodging( lodging_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::HasCapacity
// Created: JSR 2012-04-18
// -----------------------------------------------------------------------------
bool DEC_Gen_Object::HasCapacity( const std::string& capacity ) const
{
    if( type_.empty() )
        return false;
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( type_ );
    return CapacityRetriever::RetrieveCapacity( type, capacity ) != 0;
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
         >> nActivationTime_
         >> name_
         >> const_cast< MIL_Automate*& >( pTC2_ )
         >> altitudeModifier_
         >> timeLimit_
         >> mining_
         >> lodging_;
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
         << nActivationTime_
         << name_
         << const_cast< MIL_Automate*& >( pTC2_ )
         << altitudeModifier_
         << timeLimit_
         << mining_
         << lodging_;
}
