// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Intelligence.h"
#include "Entities/MIL_Formation.h"
#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"
#include "Network/NET_ASN_Messages.h"
#include "Tools/MIL_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"
#include "xeumeuleu/xml.h"

using namespace xml;

MIL_MOSIDManager MIL_Intelligence::idManager_;

BOOST_CLASS_EXPORT_GUID( MIL_Intelligence, "MIL_Intelligence" )

namespace
{
    ASN1T_EnumNatureLevel ResolveLevel( const std::string& level )
    {
        if( const PHY_NatureLevel* natureLevel = PHY_NatureLevel::Find( level ) )
            return natureLevel->GetAsnID();
        return EnumNatureLevel::none;
    }

    ASN1T_EnumDiplomacy ResolveKarma( const std::string& karma )
    {
        if( karma == "friend" )
            return EnumDiplomacy::ami;
        if( karma == "enemy" )
            return EnumDiplomacy::ennemi;
        if( karma == "neutral" )
            return EnumDiplomacy::neutre;
        return EnumDiplomacy::inconnu;
    }

    MT_Vector2D ConvertPosition( const std::string& mgrs )
    {
        MT_Vector2D pos;
        MIL_Tools::ConvertCoordMosToSim( mgrs, pos );
        return pos;
    }

    MT_Vector2D ConvertPosition( const ASN1T_CoordLatLong& mgrs )
    {
        MT_Vector2D pos;
        MIL_Tools::ConvertCoordMosToSim( mgrs, pos );
        return pos;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence constructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
MIL_Intelligence::MIL_Intelligence()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence constructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
MIL_Intelligence::MIL_Intelligence( xml::xistream& xis, MIL_Formation& formation )
    : formation_( &formation )
    , id_       ( attribute< unsigned int >( xis, "id" ) )
    , name_     ( attribute< std::string > ( xis, "name" ) )
    , nature_   ( attribute< std::string > ( xis, "nature" ) )
    , embarked_ ( attribute< bool >        ( xis, "embarked" ) )
    , level_    ( ResolveLevel( attribute< std::string >( xis, "level" ) ) )
    , diplomacy_( ResolveKarma( attribute< std::string > ( xis, "karma" ) ) )
    , position_ ( ConvertPosition( attribute< std::string >( xis, "position" ) ) )
{
    idManager_.LockMosID( id_ );
    formation_->RegisterIntelligence( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence constructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
MIL_Intelligence::MIL_Intelligence( const ASN1T_MsgIntelligenceCreationRequest& message, MIL_Formation& formation )
    : formation_( &formation )
    , id_       ( idManager_.GetFreeSimID() )
    , name_     ( message.intelligence.name )
    , nature_   ( message.intelligence.nature )
    , embarked_ ( message.intelligence.embarked ? true : false )
    , level_    ( message.intelligence.level )
    , diplomacy_( message.intelligence.diplomacy )
    , position_ ( ConvertPosition( message.intelligence.location ) )
{
    idManager_.LockMosID( id_ );
    formation_->RegisterIntelligence( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence destructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
MIL_Intelligence::~MIL_Intelligence()
{
    formation_->UnregisterIntelligence( *this );
    idManager_.ReleaseMosID( id_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::GetId
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
unsigned int MIL_Intelligence::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::Update
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void MIL_Intelligence::Update( const ASN1T_MsgIntelligenceCreationRequest& /*message*/ )
{
    SendCreation();
    {
        NET_ASN_MsgIntelligenceCreationRequestAck message;
        message().error_code = EnumIntelligenceErrorCode::no_error;
        message.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::Update
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void MIL_Intelligence::Update( const ASN1T_MsgIntelligenceUpdateRequest& message )
{
    if( message.m.namePresent )
        name_ = message.name;
    if( message.m.naturePresent )
        nature_ = message.nature;
    if( message.m.embarkedPresent )
        embarked_ = message.embarked ? true : false;
    if( message.m.levelPresent )
        level_ = message.level;
    if( message.m.diplomacyPresent )
        diplomacy_ = message.diplomacy;
    if( message.m.locationPresent )
        position_ = ConvertPosition( message.location );
    SendFullState();
    {
        NET_ASN_MsgIntelligenceUpdateRequestAck message;
        message().error_code = EnumIntelligenceErrorCode::no_error;
        message.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::Update
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void MIL_Intelligence::Update( const ASN1T_MsgIntelligenceDestructionRequest& /*message*/ )
{
    SendDestruction();
    {
        NET_ASN_MsgIntelligenceDestructionRequestAck message;
        message().error_code = EnumIntelligenceErrorCode::no_error;
        message.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::SendCreation
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void MIL_Intelligence::SendCreation() const
{
    NET_ASN_MsgIntelligenceCreation message;
    message().oid = id_;
    message().intelligence.name = name_.c_str();
    message().intelligence.nature = nature_.c_str();
    message().intelligence.embarked = embarked_ ? 1 : 0;
    message().intelligence.level = level_;
    message().intelligence.diplomacy = diplomacy_;
    message().intelligence.formation = formation_->GetID();
    MIL_Tools::ConvertCoordSimToMos( position_, message().intelligence.location );
    message.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::SendFullState
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void MIL_Intelligence::SendFullState() const
{
    NET_ASN_MsgIntelligenceUpdate message;
    message().oid = id_;
    message().formation = formation_->GetID();
    message().m.namePresent      = 1; message().name = name_.c_str();
    message().m.naturePresent    = 1; message().nature = nature_.c_str();
    message().m.embarkedPresent  = 1; message().embarked = embarked_ ? 1 : 0;
    message().m.levelPresent     = 1; message().level = level_;
    message().m.diplomacyPresent = 1; message().diplomacy = diplomacy_;
    message().m.locationPresent  = 1;
    MIL_Tools::ConvertCoordSimToMos( position_, message().location );
    message.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::SendDestruction
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void MIL_Intelligence::SendDestruction() const
{
    NET_ASN_MsgIntelligenceDestruction message;
    message().oid = id_;
    message.Send();
}

namespace
{
    std::string ResolveKarma( const ASN1T_EnumDiplomacy& diplomacy )
    {
        switch( diplomacy )
        {
        case EnumDiplomacy::ami:
            return "friend";
        case EnumDiplomacy::ennemi:
            return "enemy";
        case EnumDiplomacy::neutre:
            return "neutral";
        }
        return "unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::WriteODB
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void MIL_Intelligence::WriteODB( xml::xostream& xos ) const
{
    xos << start( "intelligence" )
            << attribute( "id", id_ )
            << attribute( "name", name_ )
            << attribute( "karma", ResolveKarma( diplomacy_ ) )
            << attribute( "level", PHY_NatureLevel::Find( level_ )->GetName() )
            << attribute( "embarked", embarked_ )
            << attribute( "nature", nature_ )
            << attribute( "position", MIL_Tools::ConvertCoordSimToMos( position_ ) )
        << end();
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::serialize
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_Intelligence::serialize( Archive& file, const uint )
{
    file & id_
         & const_cast< MIL_Formation*& >( formation_ )
         & name_
         & nature_
         & embarked_
         & level_
         & diplomacy_
         & position_;
    idManager_.LockMosID( id_ ); // $$$$ SBO 2007-10-22: 
//    formation_->RegisterIntelligence( *this ); // $$$$ SBO 2007-10-23: ?
}
