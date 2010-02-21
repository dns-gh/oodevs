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
#include "clients_kernel/CoordinateConverter.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include "protocol/MessengerSenders.h"
#include <xeumeuleu/xml.h>

using namespace plugins::messenger;

namespace
{
    Common::EnumNatureLevel ResolveLevel( const std::string& level )
    {
      if( level == "o" )
          return Common::EnumNatureLevel::o ;
      if( level == "oo" )
          return Common::EnumNatureLevel::oo ;
      if( level == "ooo" )
          return Common::EnumNatureLevel::ooo ;
      if( level == "i" )
          return Common::EnumNatureLevel::i ;
      if( level == "ii" )
          return Common::EnumNatureLevel::ii ;
      if( level == "iii" )
          return Common::EnumNatureLevel::iii ;
      if( level == "x" )
          return Common::EnumNatureLevel::x ;
      if( level == "xx" )
          return Common::EnumNatureLevel::xx ;
      if( level == "xxx" )
          return Common::EnumNatureLevel::xxx ;
      if( level == "xxxx" )
          return Common::EnumNatureLevel::xxxx ;
      return Common::EnumNatureLevel::none_naturelevel;
    }

    std::string ResolveLevel( Common::EnumNatureLevel level )
    {
        switch( level )
        {
        case Common::EnumNatureLevel::o: return "o";
        case Common::EnumNatureLevel::oo: return "oo";
        case Common::EnumNatureLevel::ooo: return "ooo";
        case Common::EnumNatureLevel::i: return "i";
        case Common::EnumNatureLevel::ii: return "ii";
        case Common::EnumNatureLevel::iii: return "iii";
        case Common::EnumNatureLevel::x: return "x";
        case Common::EnumNatureLevel::xx: return "xx";
        case Common::EnumNatureLevel::xxx: return "xxx";
        case Common::EnumNatureLevel::xxxx: return "xxxx";
        default: return "";
        }
    }

    Common::EnumDiplomacy ResolveKarma( const std::string& karma )
    {
        if( karma == "friend" )
            return Common::EnumDiplomacy::friend_diplo;
        if( karma == "enemy" )
            return Common::EnumDiplomacy::enemy_diplo;
        if( karma == "neutral" )
            return Common::EnumDiplomacy::neutral_diplo;
        return Common::EnumDiplomacy::unknown_diplo;
    }

    std::string ResolveKarma( Common::EnumDiplomacy karma )
    {
        switch( karma )
        {
        case Common::EnumDiplomacy::friend_diplo: return "friend";
        case Common::EnumDiplomacy::enemy_diplo: return "enemy";
        case Common::EnumDiplomacy::neutral_diplo: return "neutral";
        case Common::EnumDiplomacy::unknown_diplo:
        default: return "unknown";
        }
    }

}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
Intelligence::Intelligence( unsigned int id, const MsgsClientToMessenger::MsgIntelligenceCreationRequest& message )
    : id_       ( id )
    , formation_( message.intelligence().formation() )
    , name_     ( message.intelligence().name() )
    , nature_   ( message.intelligence().nature() )
    , embarked_ ( message.intelligence().embarked() ? true : false )
    , level_    ( message.intelligence().level() )
    , diplomacy_( message.intelligence().diplomacy() )
    , position_ ( message.intelligence().location() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: RDS 2008-04-08
// -----------------------------------------------------------------------------
Intelligence::Intelligence( unsigned int id, xml::xistream& xis, const Common::MsgFormation& formation, const kernel::CoordinateConverter_ABC& converter )
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
void Intelligence::Update( const MsgsClientToMessenger::MsgIntelligenceUpdateRequest& message )
{
    if( message.has_name())
        name_ = message.name();
    if( message.has_nature() )
        nature_ = message.nature();
    if( message.has_embarked() )
        embarked_ = message.embarked() ? true : false;
    if( message.has_level() )
        level_ = message.level();
    if( message.has_diplomacy() )
        diplomacy_ = message.diplomacy();
    if( message.has_location() )
        position_ = message.location();
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendUpdate
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendUpdate( dispatcher::ClientPublisher_ABC& publisher ) const
{
    plugins::messenger::IntelligenceUpdate message;
    message().set_oid       ( id_ );
    message().set_formation ( formation_.oid() );
    message().set_name(name_.c_str());
    message().set_nature(nature_.c_str());
    message().set_embarked(embarked_ ? 1 : 0);
    message().set_level(level_);
    message().set_diplomacy(diplomacy_);
    *message().mutable_location() = position_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendCreation
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    plugins::messenger::IntelligenceCreation message;
    message().set_oid ( id_ );
    message().mutable_intelligence()->set_name( name_ );
    message().mutable_intelligence()->set_nature( nature_ );
    message().mutable_intelligence()->set_embarked( embarked_ );
    message().mutable_intelligence()->set_level( level_ );
    message().mutable_intelligence()->set_diplomacy( diplomacy_ );
    *message().mutable_intelligence()->mutable_formation() = formation_;
    *message().mutable_intelligence()->mutable_location() = position_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendDestruction
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const
{
    plugins::messenger::IntelligenceDestruction message;
    message().set_oid ( id_ );
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
    xis >> xml::attribute( "position", mgrs );
    geometry::Point2d pos = converter.ConvertToGeo( converter.ConvertToXY( mgrs ) );
    position_.set_latitude( pos.Y() );
    position_.set_longitude( pos.X() );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void Intelligence::Write( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const
{
    xos << xml::start( "intelligence" )
            << xml::attribute( "id", id_ )
            << xml::attribute( "name", name_ )
            << xml::attribute( "embarked", embarked_ )
            << xml::attribute( "level", ResolveLevel( level_ ) )
            << xml::attribute( "karma", ResolveKarma( diplomacy_ ) )
            << xml::attribute( "nature", nature_ );

    WritePosition( xos, converter );

    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Intelligence::WritePosition
// Created: RDS 2008-04-10
// -----------------------------------------------------------------------------
void Intelligence::WritePosition( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const
{
    geometry::Point2d pos( position_.longitude(), position_.latitude() );
    xos << xml::attribute( "position", converter.ConvertToMgrs( converter.ConvertFromGeo( pos ) ) );
}
