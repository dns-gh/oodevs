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
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/CapacityRetriever.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/NET_AsnException.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object()
    : identifier_( 0 )
    , activated_( true )
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

namespace
{
    bool ExtractActivated( const sword::PlannedWork& msg )
    {
        if( msg.has_type_obstacle() && msg.type_obstacle() == sword::ObstacleType_DemolitionTargetType_reserved )
            return false;
        return true;
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
    , activated_         ( ExtractActivated( msg ) )
    , rDensity_          ( msg.has_density() ? msg.density() : 0. )
    , nMinesActivityTime_( msg.has_activity_time() ? msg.activity_time() : 0 )
    , nActivationTime_   ( msg.has_activation_time() ? msg.activation_time() : 0 )
    , name_              ( msg.has_name() ? msg.name() : std::string() )
    , pTC2_              ( 0 )
    , altitudeModifier_  ( msg.has_altitude_modifier() ? msg.altitude_modifier() : 0 )
    , timeLimit_         ( msg.has_time_limit() ? msg.time_limit() : 0 )
    , lodging_           ( msg.has_lodging() ? msg.lodging() : 0 )
    , mining_            ( msg.has_mining() ? msg.mining() : false )
    , fireClass_         ( msg.has_fire_class() ? msg.fire_class() : "" )
    , maxCombustion_     ( msg.has_max_combustion() ? msg.max_combustion() : 0 )
{
    if( type_.empty() )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
    if( !NET_ASN_Tools::ReadLocation( msg.position(), localisation_ ) )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
    SetLocationSize( resolver, type_, localisation_ );
    if( msg.combat_train().id() != 0 )
    {
        pTC2_ = entityManager.FindAutomate( msg.combat_train().id() );
        if( !pTC2_ )
            throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
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
    , activated_         ( ExtractActivated( msg ) )
    , rDensity_          ( msg.has_density() ? msg.density() : 0. )
    , nMinesActivityTime_( msg.has_activity_time() ? msg.activity_time() : 0 )
    , nActivationTime_   ( msg.has_activation_time() ? msg.activation_time() : 0 )
    , name_              ( msg.has_name() ? msg.name() : std::string() )
    , pTC2_              ( 0 )
    , altitudeModifier_  ( msg.has_altitude_modifier() ? msg.altitude_modifier() : 0 )
    , timeLimit_         ( msg.has_time_limit() ? msg.time_limit() : 0 )
    , lodging_           ( msg.has_lodging() ? msg.lodging() : 0 )
    , mining_            ( msg.has_mining() ? msg.mining() : false )
    , fireClass_         ( msg.has_fire_class() ? msg.fire_class() : "" )
    , maxCombustion_     ( msg.has_max_combustion() ? msg.max_combustion() : 0 )
{
    if( type_.empty() )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
    if( !NET_ASN_Tools::ReadLocation( msg.position(), localisation_ ) )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
    SetLocationSize( resolver, type_, localisation_ );
    if( msg.combat_train().id() != 0 )
    {
        pTC2_ = entityManager.FindAutomate( msg.combat_train().id() );
        if( !pTC2_ )
            throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: MGD 2010-06-16
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( std::string type, TER_Localisation* location, bool activated )
    : type_              ( type )
    , identifier_        ( 0u )
    , localisation_      ( *location )
    , activated_         ( activated )
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
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( const DEC_Gen_Object& rhs )
    : type_              ( rhs.type_ )
    , identifier_        ( rhs.identifier_ )
    , localisation_      ( rhs.localisation_ )
    , activated_         ( rhs.activated_ )
    , rDensity_          ( rhs.rDensity_ )
    , nMinesActivityTime_( rhs.nMinesActivityTime_ )
    , nActivationTime_   ( rhs.nActivationTime_ )
    , name_              ( rhs.name_ )
    , pTC2_              ( rhs.pTC2_ )
    , altitudeModifier_  ( rhs.altitudeModifier_ )
    , timeLimit_         ( rhs.timeLimit_ )
    , lodging_           ( 0 )
    , mining_            ( rhs.mining_ )
    , fireClass_         ( rhs.fireClass_ )
    , maxCombustion_     ( rhs.maxCombustion_ )
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
    activated_          = rhs.activated_;
    rDensity_           = rhs.rDensity_;
    nMinesActivityTime_ = rhs.nMinesActivityTime_;
    nActivationTime_    = rhs.nActivationTime_;
    name_               = rhs.name_;
    pTC2_               = rhs.pTC2_;
    altitudeModifier_   = rhs.altitudeModifier_;
    timeLimit_          = rhs.timeLimit_;
    lodging_            = rhs.lodging_;
    mining_             = rhs.mining_;
    fireClass_          = rhs.fireClass_;
    maxCombustion_      = rhs.maxCombustion_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::Serialize
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
void DEC_Gen_Object::Serialize( sword::PlannedWork& msg ) const
{
    msg.set_type( type_.c_str() );
    msg.set_type_obstacle( activated_ ? sword::ObstacleType_DemolitionTargetType_preliminary : sword::ObstacleType_DemolitionTargetType_reserved );
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
    if( !fireClass_.empty() )
        msg.set_fire_class( fireClass_ );
    msg.set_max_combustion( maxCombustion_ );
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
    file >> type_;
    file >> identifier_;
    file >> localisation_;
    file >> activated_;
    file >> rDensity_;
    file >> nMinesActivityTime_;
    file >> nActivationTime_;
    file >> name_;
    file >> const_cast< MIL_Automate*& >( pTC2_ );
    file >> altitudeModifier_;
    file >> timeLimit_;
    file >> mining_;
    file >> lodging_;
    file >> fireClass_;
    file >> maxCombustion_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::save
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void DEC_Gen_Object::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << type_;
    file << identifier_;
    file << localisation_;
    file << activated_;
    file << rDensity_;
    file << nMinesActivityTime_;
    file << nActivationTime_;
    file << name_;
    file << const_cast< MIL_Automate*& >( pTC2_ );
    file << altitudeModifier_;
    file << timeLimit_;
    file << mining_;
    file << lodging_;
    file << fireClass_;
    file << maxCombustion_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetType
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
const std::string& DEC_Gen_Object::GetTypeName() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetExternalIdentifier
// Created: LGY 2011-10-25
// -----------------------------------------------------------------------------
unsigned int DEC_Gen_Object::GetExternalIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetLocalisation
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
const TER_Localisation& DEC_Gen_Object::GetLocalisation() const
{
    return localisation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetActivated
// Created: JSR 2013-09-18
// -----------------------------------------------------------------------------
bool DEC_Gen_Object::GetActivated() const
{
    return activated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetDensity
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
double DEC_Gen_Object::GetDensity() const
{
    return rDensity_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetTC2
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
const MIL_Automate* DEC_Gen_Object::GetTC2() const
{
    return pTC2_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetMinesActivityTime
// Created: NLD 2007-02-06
// -----------------------------------------------------------------------------
unsigned int DEC_Gen_Object::GetMinesActivityTime() const
{
    return nMinesActivityTime_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetActivationTime
// Created: LDC 2012-01-24
// -----------------------------------------------------------------------------
unsigned int DEC_Gen_Object::GetActivationTime() const
{
    return nActivationTime_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetName
// Created: LDC 2012-01-24
// -----------------------------------------------------------------------------
const std::string& DEC_Gen_Object::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetAltitudeModifier
// Created: MMC 2012-04-20
// -----------------------------------------------------------------------------
int DEC_Gen_Object::GetAltitudeModifier() const
{
    return altitudeModifier_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetTimeLimit
// Created: MMC 2012-04-20
// -----------------------------------------------------------------------------
int DEC_Gen_Object::GetTimeLimit() const
{
    return timeLimit_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetLodging
// Created: MMC 2012-07-04
// -----------------------------------------------------------------------------
int DEC_Gen_Object::GetLodging() const
{
    return lodging_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetMining
// Created: MMC 2013-06-18
// -----------------------------------------------------------------------------
bool DEC_Gen_Object::GetMining() const
{
    return mining_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetMining
// Created: LGy 2013-11-26
// -----------------------------------------------------------------------------
const std::string& DEC_Gen_Object::GetFireClass() const
{
    return fireClass_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetMaxCombustion
// Created: LGy 2013-11-26
// -----------------------------------------------------------------------------
unsigned int DEC_Gen_Object::GetMaxCombustion() const
{
    return maxCombustion_;
}
