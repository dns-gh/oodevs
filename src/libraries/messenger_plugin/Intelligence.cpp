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
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/MessengerSenders.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

namespace
{
    sword::EnumNatureLevel ResolveLevel( const std::string& level )
    {
      if( level == "b" )
          return sword::b ;
      if( level == "o" )
          return sword::o ;
      if( level == "c" )
          return sword::c ;
      if( level == "oo" )
          return sword::oo ;
      if( level == "ooo" )
          return sword::ooo ;
      if( level == "i" )
          return sword::i ;
      if( level == "ii" )
          return sword::ii ;
      if( level == "iii" )
          return sword::iii ;
      if( level == "x" )
          return sword::x ;
      if( level == "xx" )
          return sword::xx ;
      if( level == "xxx" )
          return sword::xxx ;
      if( level == "xxxx" )
          return sword::xxxx ;
      return sword::none_naturelevel;
    }

    std::string ResolveLevel( sword::EnumNatureLevel level )
    {
        switch( level )
        {
        case sword::b: return "b";
        case sword::o: return "o";
        case sword::c: return "c";
        case sword::oo: return "oo";
        case sword::ooo: return "ooo";
        case sword::i: return "i";
        case sword::ii: return "ii";
        case sword::iii: return "iii";
        case sword::x: return "x";
        case sword::xx: return "xx";
        case sword::xxx: return "xxx";
        case sword::xxxx: return "xxxx";
        default: return "";
        }
    }

    sword::EnumDiplomacy ResolveKarma( const std::string& karma )
    {
        if( karma == "friend" )
            return sword::friendly;
        if( karma == "enemy" )
            return sword::enemy;
        if( karma == "neutral" )
            return sword::neutral;
        return sword::unknown;
    }

    std::string ResolveKarma( sword::EnumDiplomacy karma )
    {
        switch( karma )
        {
        case sword::friendly: return "friend";
        case sword::enemy: return "enemy";
        case sword::neutral: return "neutral";
        case sword::unknown:
        default: return "unknown";
        }
    }

}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
Intelligence::Intelligence( unsigned int id, const sword::IntelligenceCreationRequest& message )
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
Intelligence::Intelligence( unsigned int id, xml::xistream& xis, const sword::FormationId& formation, const kernel::CoordinateConverter_ABC& converter )
    : id_       ( id )
    , name_     ( xis.attribute< std::string >( "name" ) )
    , nature_   ( xis.attribute< std::string >( "nature" ) )
    , embarked_ ( xis.attribute< bool >( "embarked" ) )
    , formation_( formation )
    , level_    ( ResolveLevel( xis.attribute< std::string >( "level" ) ) )
    , diplomacy_( ResolveKarma( xis.attribute< std::string >( "karma" ) ) )
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
void Intelligence::Update( const sword::IntelligenceUpdateRequest& message )
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
    message().mutable_id()->set_id( id_ );
    message().mutable_formation()->set_id( formation_.id() );
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
    message().mutable_id()->set_id( id_ );
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
    message().mutable_id()->set_id( id_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendFullState
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void Intelligence::SendFullState( dispatcher::ClientPublisher_ABC& publisher ) const
{
    SendCreation( publisher );
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
    xos << xml::end;
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
