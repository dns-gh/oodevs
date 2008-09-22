// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "Intelligence.h"

#include "dispatcher/ClientPublisher_ABC.h"
#include "clients_kernel/CoordinateConverter.h"

#include <xeumeuleu/xml.h>

using namespace messenger;

namespace
{
    ASN1T_EnumNatureLevel ResolveLevel( const std::string& level )
    {
      if( level == "o" )
          return EnumNatureLevel::o ;
      if( level == "oo" )
          return EnumNatureLevel::oo ;
      if( level == "ooo" )
          return EnumNatureLevel::ooo ;
      if( level == "i" )
          return EnumNatureLevel::i ;
      if( level == "ii" )
          return EnumNatureLevel::ii ;
      if( level == "iii" )
          return EnumNatureLevel::iii ;
      if( level == "x" )
          return EnumNatureLevel::x ;
      if( level == "xx" )
          return EnumNatureLevel::xx ;
      if( level == "xxx" )
          return EnumNatureLevel::xxx ;
      if( level == "xxxx" )
          return EnumNatureLevel::xxxx ;
      return EnumNatureLevel::none;
    }

    std::string ResolveLevel(ASN1T_EnumNatureLevel level)
    {
        switch (level)
        {
        case EnumNatureLevel::o: return "o";
        case EnumNatureLevel::oo: return "oo";
        case EnumNatureLevel::ooo: return "ooo";
        case EnumNatureLevel::i: return "i";
        case EnumNatureLevel::ii: return "ii";
        case EnumNatureLevel::iii: return "iii";
        case EnumNatureLevel::x: return "x";
        case EnumNatureLevel::xx: return "xx";
        case EnumNatureLevel::xxx: return "xxx";
        case EnumNatureLevel::xxxx: return "xxxx";
        default: return "";
        }
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

    std::string ResolveKarma(ASN1T_EnumDiplomacy karma)
    {
        switch(karma)
        {
        case EnumDiplomacy::ami: return "friend";
        case EnumDiplomacy::ennemi: return "enemy";
        case EnumDiplomacy::neutre: return "neutral";
        case EnumDiplomacy::inconnu:
        default: return "unknown";
        }
    }

}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
Intelligence::Intelligence( unsigned int id, const ASN1T_MsgIntelligenceCreationRequest& message )
    : id_       ( id )
    , formation_( message.intelligence.formation )
    , name_     ( message.intelligence.name )
    , nature_   ( message.intelligence.nature )
    , embarked_ ( message.intelligence.embarked ? true : false )
    , level_    ( message.intelligence.level )
    , diplomacy_( message.intelligence.diplomacy )
    , position_ ( message.intelligence.location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: RDS 2008-04-08
// -----------------------------------------------------------------------------
Intelligence::Intelligence( unsigned int id, xml::xistream& xis, const ASN1T_Formation& formation, const kernel::CoordinateConverter_ABC& converter )
    : id_       ( id )
    , name_     ( xml::attribute< std::string > ( xis, "name" ) )
    , nature_   ( xml::attribute< std::string > ( xis, "nature" ) )
    , embarked_ ( xml::attribute< bool >        ( xis, "embarked" ) )
    , formation_( formation )
    , level_    ( ResolveLevel( xml::attribute< std::string >( xis, "level" ) ) )
    , diplomacy_( ResolveKarma( xml::attribute< std::string >( xis, "karma" ) ) )
{
    ReadPosition(xis,converter);
}

// -----------------------------------------------------------------------------
// Name: Intelligence destructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
Intelligence::~Intelligence()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Update
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Intelligence::Update( const ASN1T_MsgIntelligenceUpdateRequest& message )
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
        position_ = message.location;
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendUpdate
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendUpdate( dispatcher::ClientPublisher_ABC& publisher ) const
{
    messenger::IntelligenceUpdate message;
    message().oid       = id_;
    message().formation = formation_ ;
    message().m.namePresent      = 1; message().name      = name_.c_str();
    message().m.naturePresent    = 1; message().nature    = nature_.c_str();
    message().m.embarkedPresent  = 1; message().embarked  = embarked_ ? 1 : 0;
    message().m.levelPresent     = 1; message().level     = level_;
    message().m.diplomacyPresent = 1; message().diplomacy = diplomacy_;
    message().m.locationPresent  = 1; message().location  = position_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendCreation
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    messenger::IntelligenceCreation message;
    message().oid = id_;
    message().intelligence.name      = name_.c_str();
    message().intelligence.nature    = nature_.c_str();
    message().intelligence.embarked  = embarked_ ? 1 : 0;
    message().intelligence.level     = level_;
    message().intelligence.diplomacy = diplomacy_;
    message().intelligence.formation = formation_ ;
    message().intelligence.location  = position_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendDestruction
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const
{
    messenger::IntelligenceDestruction message;
    message().oid = id_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendFullState
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void Intelligence::SendFullState( dispatcher::ClientPublisher_ABC& publisher ) const
{
    SendCreation(publisher);
}

// -----------------------------------------------------------------------------
// Name: Intelligence::ReadPosition
// Created: RDS 2008-04-10
// -----------------------------------------------------------------------------
void Intelligence::ReadPosition( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
{
    std::string mgrs ;
    xis >> xml::attribute("position", mgrs);
    geometry::Point2d pos = converter.ConvertToGeo(converter.ConvertToXY(mgrs));
    position_.latitude = pos.Y();
    position_.longitude = pos.X();
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void Intelligence::Write( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const
{
    xos << xml::start("intelligence")
            << xml::attribute("id",id_)
            << xml::attribute("name",name_)
            << xml::attribute("embarked",embarked_)
            << xml::attribute("level",ResolveLevel(level_))
            << xml::attribute("karma",ResolveKarma(diplomacy_))
            << xml::attribute("nature",nature_);

    WritePosition(xos,converter);

    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Intelligence::WritePosition
// Created: RDS 2008-04-10
// -----------------------------------------------------------------------------
void Intelligence::WritePosition( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const
{
    geometry::Point2d pos(position_.longitude,position_.latitude);
    xos << xml::attribute("position",converter.ConvertToMgrs(converter.ConvertFromGeo(pos)));
}
