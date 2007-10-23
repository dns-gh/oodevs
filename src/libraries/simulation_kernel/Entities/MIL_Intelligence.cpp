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
        return EnumDiplomacy::neutre; // $$$$ SBO 2007-10-22: do not allow "unknown" as a karma
    }

    MT_Vector2D ConvertPosition( const std::string& mgrs )
    {
        MT_Vector2D pos;
        MIL_Tools::ConvertCoordMosToSim( mgrs, pos );
        return pos;
    }

    MT_Vector2D ConvertPosition( const ASN1T_CoordUTM& mgrs )
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
MIL_Intelligence::MIL_Intelligence( xml::xistream& xis, const MIL_Formation& formation )
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
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence constructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
MIL_Intelligence::MIL_Intelligence( const ASN1T_MsgIntelligenceCreationRequest& message, const MIL_Formation& formation )
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
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence destructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
MIL_Intelligence::~MIL_Intelligence()
{
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
// Name: MIL_Intelligence::SendCreation
// Created: SBO 2007-10-22
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
    message().intelligence.diffusion.t = T_IntelligenceDiffusion_formation;
    message().intelligence.diffusion.u.formation = formation_->GetID();
    const std::string mgrs = MIL_Tools::ConvertCoordSimToMos( position_ );
    message().intelligence.location = mgrs.c_str();
    message.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::SendDestruction
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void MIL_Intelligence::SendDestruction() const
{
    NET_ASN_MsgIntelligenceDestruction message;
    message().oid = id_;
    message.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::SendFullState
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void MIL_Intelligence::SendFullState() const
{
    // $$$$ SBO 2007-10-22: TODO ?
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
        }
        return "neutral";
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Intelligence::WriteODB
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void MIL_Intelligence::WriteODB( xml::xostream& xos ) const
{
    xos << attribute( "id", id_ )
        << attribute( "name", name_ )
        << attribute( "karma", ResolveKarma( diplomacy_ ) )
        << attribute( "level", PHY_NatureLevel::Find( level_ )->GetName() )
        << attribute( "embarked", embarked_ )
        << attribute( "type" , nature_ )
        << attribute( "position", MIL_Tools::ConvertCoordSimToMos( position_ ) );
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
}
