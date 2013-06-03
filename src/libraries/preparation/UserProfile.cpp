// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UserProfile.h"
#include "AgentsModel.h"
#include "FormationModel.h"
#include "GhostModel.h"
#include "Model.h"
#include "TeamsModel.h"
#include "TacticalHierarchies.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/DictionaryType.h"
#include "clients_kernel/Team_ABC.h"
#include "tools/Iterator.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::UserProfile( xml::xistream& xis, kernel::Controller& controller, const Model& model,
                          const kernel::ExtensionTypes& extensions )
    : controller_( controller )
    , model_     ( model )
    , extensions_( extensions )
    , isClone_   ( false )
    , isLowLevel_( false )
    , controlTime_( false )
{
    const ExistenceChecker< tools::Resolver_ABC< kernel::Team_ABC > >       teamChecker( model_.GetTeamResolver() );
    const ExistenceChecker< tools::Resolver_ABC< kernel::Formation_ABC > >  formationChecker( model_.GetFormationResolver() );
    const ExistenceChecker< tools::Resolver_ABC< kernel::Automat_ABC > >    automatChecker( model_.GetAutomatResolver() );
    const ExistenceChecker< tools::Resolver_ABC< kernel::Population_ABC > > populationChecker( model_.GetPopulationResolver() );
    const ExistenceChecker< tools::Resolver_ABC< kernel::Ghost_ABC > >      ghostChecker( model_.ghosts_ );

    std::string login, pass;
    xis >> xml::attribute( "name", login )
        >> xml::attribute( "password", pass )
        >> xml::optional >> xml::attribute( "role", userRole_ )
        >> xml::optional >> xml::attribute( "time-control", controlTime_ )
        >> xml::start( "rights" )
            >> xml::start( "readonly" )
                >> xml::list( "side"      , *this, &UserProfile::ReadRights, readSides_, teamChecker )
                >> xml::list( "formation" , *this, &UserProfile::ReadRights, readFormations_, formationChecker )
                >> xml::list( "automat"   , *this, &UserProfile::ReadAutomatRights, readAutomats_, automatChecker, readGhosts_, ghostChecker )
                >> xml::list( "crowd"     , *this, &UserProfile::ReadRights, readPopulations_, populationChecker )
            >> xml::end
            >> xml::start( "readwrite" )
                >> xml::list( "side"      , *this, &UserProfile::ReadRights, writeSides_, teamChecker )
                >> xml::list( "formation" , *this, &UserProfile::ReadRights, writeFormations_, formationChecker )
                >> xml::list( "automat"   , *this, &UserProfile::ReadAutomatRights, writeAutomats_, automatChecker, writeGhosts_, ghostChecker )
                >> xml::list( "crowd"     , *this, &UserProfile::ReadRights, writePopulations_, populationChecker )
            >> xml::end
        >> xml::end;
    login_ = login.c_str();
    password_ = pass.c_str();
    ComputeLowLevel();
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const QString& login, kernel::Controller& controller, const Model& model,
                          const kernel::ExtensionTypes& extensions )
    : controller_( controller )
    , model_     ( model )
    , extensions_( extensions )
    , login_     ( login )
    , password_  ( "" )
    , isClone_   ( false )
    , isLowLevel_( false )
    , controlTime_( false )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const QString& login, const std::string& role, kernel::Controller& controller,
                          const Model& model, const kernel::ExtensionTypes& extensions )
    : controller_ ( controller )
    , model_      ( model )
    , extensions_ ( extensions )
    , login_      ( login )
    , password_   ( "" )
    , isClone_    ( false )
    , isLowLevel_ ( false )
    , userRole_   ( role )
    , controlTime_( false )
{
    ComputeLowLevel();
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const UserProfile& p )
    : controller_      ( p.controller_ )
    , model_           ( p.model_ )
    , login_           ( p.login_ )
    , extensions_      ( p.extensions_ )
    , password_        ( p.password_ )
    , readSides_       ( p.readSides_ )
    , readFormations_  ( p.readFormations_ )
    , readAutomats_    ( p.readAutomats_ )
    , readPopulations_ ( p.readPopulations_ )
    , readGhosts_      ( p.readGhosts_ )
    , writeSides_      ( p.writeSides_ )
    , writeFormations_ ( p.writeFormations_ )
    , writeAutomats_   ( p.writeAutomats_ )
    , writePopulations_( p.writePopulations_ )
    , writeGhosts_     ( p.writeGhosts_ )
    , isClone_         ( true )
    , isLowLevel_      ( p.isLowLevel_ )
    , userRole_        ( p.userRole_ )
    , controlTime_     ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfile destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::~UserProfile()
{
    if( !isClone_ )
        controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::Serialize
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfile::Serialize( xml::xostream& xos ) const
{
    T_Ids readSides;
    T_Ids readFormations;
    T_Ids readAutomats;
    T_Ids readPopulations;
    T_Ids writeSides;
    T_Ids writeFormations;
    T_Ids writeAutomats;
    T_Ids writePopulations;

    bool testProfile = userRole_.empty();

    if( HasProperty( "readAll" ) )
    {
        tools::Iterator< const kernel::Team_ABC& > itReceiver = model_.teams_.CreateIterator();
        if( !HasProperty( "noEditable") )
        {
            while( itReceiver.HasMoreElements() )
            {
                unsigned long id = itReceiver.NextElement().GetId();
                bool readwrite = false;
                for( T_Ids::const_iterator it = writeSides_.begin(); it != writeSides_.end(); ++it )
                {
                    if( id == *it )
                    {
                        readwrite = true;
                        break;
                    }
                }
                if( !readwrite )
                    readSides.push_back( id );
            }
            writeSides = writeSides_;
            writeFormations = writeFormations_;
            writeAutomats = writeAutomats_;
            writePopulations = writePopulations_;
        }
        else
            while( itReceiver.HasMoreElements() )
                readSides.push_back( itReceiver.NextElement().GetId() );
    }
    else if( testProfile || HasProperty( "writeAll" ) )
    {
        tools::Iterator< const kernel::Team_ABC& > itReceiver = model_.teams_.CreateIterator();
        while( itReceiver.HasMoreElements() )
        {
            const unsigned int id = itReceiver.NextElement().GetId();
            writeSides.push_back( id );
        }
    }
    else
    {
        readSides = readSides_;
        readFormations = readFormations_;
        readAutomats = readAutomats_;
        readPopulations = readPopulations_;
        writeSides = writeSides_;
        writeFormations = writeFormations_;
        writeAutomats = writeAutomats_;
        writePopulations = writePopulations_;
    }
    
    xos << xml::start( "profile" )
            << xml::attribute( "role", userRole_ )
            << xml::attribute( "name", login_.toAscii().constData() )
            << xml::attribute( "password", password_.toAscii().constData() )
            << xml::attribute( "time-control", controlTime_ || testProfile )
            << xml::attribute( "supervision", HasProperty( "magicActions" ) || testProfile )
            << xml::start( "rights" )
                << xml::start( "readonly" );
    SerializeRights( xos, "side", readSides );
    SerializeRights( xos, "automat", readAutomats );
    SerializeRights( xos, "crowd", readPopulations );
    xos         << xml::end
                << xml::start( "readwrite" );
    SerializeRights( xos, "side", writeSides );
    SerializeRights( xos, "automat", writeAutomats );
    SerializeRights( xos, "crowd", writePopulations );
    xos         << xml::end
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::HasProperty
// Created: LGY 2011-12-15
// -----------------------------------------------------------------------------
bool UserProfile::HasProperty( const std::string& name ) const
{
    std::vector< std::string > result;
    kernel::ExtensionType* type = extensions_.tools::StringResolver< kernel::ExtensionType >::Find( "profile-attributes" );
    if( type )
    {
        tools::Iterator< const kernel::AttributeType& > it = type->CreateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::AttributeType& attribute = it.NextElement();
            if( attribute.GetType() == kernel::AttributeType::ETypeDictionary )
            {
                std::string dictionary, dicoKind, dicoLanguage;
                attribute.GetDictionaryValues( dictionary, dicoKind, dicoLanguage );
                kernel::DictionaryType* userRoleDico = extensions_.tools::StringResolver< kernel::DictionaryType >::Find( dictionary );
                if( userRoleDico )
                    userRoleDico->GetStringList( result, dicoKind, dicoLanguage, name );
            }
        }
    }
    return std::find( result.begin(), result.end(), userRole_ ) != result.end();
}

// -----------------------------------------------------------------------------
// Name: UserProfile::ReadRights
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::ReadRights( xml::xistream& xis, T_Ids& list, const ExistenceChecker_ABC& checker )
{
    unsigned long id;
    xis >> xml::attribute( "id", id );
    if( checker( id ) )
        list.push_back( id );
    else
        std::cerr << "Invalid entity id in profile: " << id << std::endl; // $$$$ SBO 2007-11-06:
}

// -----------------------------------------------------------------------------
// Name: UserProfile::ReadAutomatRights
// Created: ABR 2012-06-27
// -----------------------------------------------------------------------------
void UserProfile::ReadAutomatRights( xml::xistream& xis, T_Ids& list, const ExistenceChecker_ABC& checker, T_Ids& ghostList, const ExistenceChecker_ABC& ghostChecker )
{
    unsigned long id;
    xis >> xml::attribute( "id", id );
    if( checker( id ) )
        list.push_back( id );
    else if( ghostChecker( id ) )
        ghostList.push_back( id );
    else
        std::cerr << "Invalid entity id in profile: " << id << std::endl; // $$$$ SBO 2007-11-06:
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SerializeRights
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfile::SerializeRights( xml::xostream& xos, const std::string& tag, const T_Ids& list ) const
{
    for( T_Ids::const_iterator it = list.begin(); it != list.end(); ++it )
        xos << xml::start( tag )
                << xml::attribute( "id", *it )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetLogin
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
QString UserProfile::GetLogin() const
{
    return login_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetPassword
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
QString UserProfile::GetPassword() const
{
    return password_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetRole
// Created: NPT 2013-05-21
// -----------------------------------------------------------------------------
QString UserProfile::GetRole() const
{
    return userRole_.c_str();
}

// -----------------------------------------------------------------------------
// Name: UserProfile::CanControlTime
// Created: NPT 2013-05-21
// -----------------------------------------------------------------------------
bool UserProfile::CanControlTime() const
{
    return controlTime_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsLowLevel
// Created: LDC 2012-04-30
// -----------------------------------------------------------------------------
bool UserProfile::IsLowLevel() const
{
    return isLowLevel_;
}

namespace
{
    template< typename List >
    bool FindIn( unsigned long id, const List& list )
    {
        return std::find( list.begin(), list.end(), id ) != list.end();
    }
}
// -----------------------------------------------------------------------------
// Name: UserProfile::IsReadable
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
bool UserProfile::IsReadable( const kernel::Entity_ABC& entity ) const
{
    return FindIn( entity.GetId(), readSides_ )
        || FindIn( entity.GetId(), readFormations_ )
        || FindIn( entity.GetId(), readAutomats_ )
        || FindIn( entity.GetId(), readPopulations_ )
        || FindIn( entity.GetId(), readGhosts_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsWriteable
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
bool UserProfile::IsWriteable( const kernel::Entity_ABC& entity ) const
{
    return FindIn( entity.GetId(), writeSides_ )
        || FindIn( entity.GetId(), writeFormations_ )
        || FindIn( entity.GetId(), writeAutomats_ )
        || FindIn( entity.GetId(), writePopulations_ )
        || FindIn( entity.GetId(), writeGhosts_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetUserRole
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
const std::string& UserProfile::GetUserRole() const
{
    return userRole_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetLogin
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetLogin( const QString& value )
{
    login_ = value;
    if( !isClone_ )
        controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetPassword
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetPassword( const QString& value )
{
    password_ = value;
    if( !isClone_ )
        controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetRight
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfile::SetRight( unsigned long id, T_Ids& ids, bool status )
{
    T_Ids::iterator it = std::find( ids.begin(), ids.end(), id );
    if( it == ids.end() && status )
        ids.push_back( id );
    else if( it != ids.end() && !status )
        ids.erase( it );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetReadable
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetReadable( const kernel::Entity_ABC& entity, bool readable )
{
    const unsigned long id = entity.GetId();
    if( dynamic_cast< const kernel::Team_ABC* >( &entity ) )
        SetRight( id, readSides_, readable );
    else if( dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        SetRight( id, readFormations_, readable );
    else if( dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        SetRight( id, readAutomats_, readable );
    else if( dynamic_cast< const kernel::Population_ABC* >( &entity ) )
        SetRight( id, readPopulations_, readable );
    else if( dynamic_cast< const kernel::Ghost_ABC* >( &entity ) )
        SetRight( id, readGhosts_, readable );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetWriteable
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetWriteable( const kernel::Entity_ABC& entity, bool writeable )
{
    const unsigned long id = entity.GetId();
    if( dynamic_cast< const kernel::Team_ABC* >( &entity ) )
        SetRight( id, writeSides_, writeable );
    else if( dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        SetRight( id, writeFormations_, writeable );
    else if( dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        SetRight( id, writeAutomats_, writeable );
    else if( dynamic_cast< const kernel::Population_ABC* >( &entity ) )
        SetRight( id, writePopulations_, writeable );
    else if( dynamic_cast< const kernel::Ghost_ABC* >( &entity ) )
        SetRight( id, writeGhosts_, writeable );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetUserRole
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void UserProfile::SetUserRole( const std::string& role )
{
    userRole_ = role;
    ComputeLowLevel();
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetTimeControl
// Created: NPT 2013-05-22
// -----------------------------------------------------------------------------
void UserProfile::SetTimeControl( bool control )
{
    controlTime_ = control;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::Clear
// Created: LGY 2011-12-15
// -----------------------------------------------------------------------------
void UserProfile::Clear()
{
    readSides_.clear();
    readFormations_.clear();
    readAutomats_.clear();
    readPopulations_.clear();
    readGhosts_.clear();
    writeSides_.clear();
    writeFormations_.clear();
    writeAutomats_.clear();
    writePopulations_.clear();
    writeGhosts_.clear();
}

// -----------------------------------------------------------------------------
// Name: UserProfile::operator=
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
UserProfile& UserProfile::operator=( const UserProfile& p )
{
    const bool changed = login_ != p.login_ || password_ != p.password_;
    login_            = p.login_;
    password_         = p.password_;
    readSides_        = p.readSides_;
    readFormations_   = p.readFormations_;
    readAutomats_     = p.readAutomats_;
    readPopulations_  = p.readPopulations_;
    readGhosts_       = p.readGhosts_;
    writeSides_       = p.writeSides_;
    writeFormations_  = p.writeFormations_;
    writeAutomats_    = p.writeAutomats_;
    writePopulations_ = p.writePopulations_;
    writeGhosts_      = p.writeGhosts_;
    userRole_         = p.userRole_;
    isLowLevel_       = p.isLowLevel_;
    if( !isClone_ && changed )
        controller_.Update( *this );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyTeamDeleted( unsigned long teamId )
{
    SetRight( teamId, readSides_, false );
    SetRight( teamId, writeSides_, false );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyFormationDeleted( unsigned long formationId )
{
    SetRight( formationId, readFormations_, false );
    SetRight( formationId, writeFormations_, false );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyAutomatDeleted( unsigned long automatId )
{
    SetRight( automatId, readAutomats_, false );
    SetRight( automatId, writeAutomats_, false );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyPopulationDeleted( unsigned long populationId )
{
    SetRight( populationId, readPopulations_, false );
    SetRight( populationId, writePopulations_, false );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyGhostDeleted
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
void UserProfile::NotifyGhostDeleted( unsigned long ghostId )
{
    SetRight( ghostId, readGhosts_, false );
    SetRight( ghostId, writeGhosts_, false );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::Visit
// Created: LGY 2011-09-16
// -----------------------------------------------------------------------------
void UserProfile::Visit( std::vector< unsigned long >& elements ) const
{
    elements.insert( elements.begin(), writeSides_.begin(), writeSides_.end() );
    elements.insert( elements.begin(), writeFormations_.begin(), writeFormations_.end() );
    elements.insert( elements.begin(), writeAutomats_.begin(), writeAutomats_.end() );
    elements.insert( elements.begin(), writePopulations_.begin(), writePopulations_.end() );
    elements.insert( elements.begin(), writeGhosts_.begin(), writeGhosts_.end() );
}

namespace{
    void InsertAutomats( std::set< unsigned long >& automats, const kernel::Entity_ABC& entity, const Model& model )
    {
        const kernel::TacticalHierarchies* hirearchies = entity.Retrieve< kernel::TacticalHierarchies >();
        if( hirearchies )
        {
            tools::Iterator< const kernel::Entity_ABC& > it = hirearchies->CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& childEntity = it.NextElement();
                const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &childEntity );
                if( pAutomat )
                    automats.insert( pAutomat->GetId() );
                else
                    InsertAutomats( automats, childEntity, model );
            }
        }
    }

    void InsertAutomatsFromTeams( const std::vector< unsigned long >& teams, std::set< unsigned long >& automats, const Model& model )
    {
        for( std::vector< unsigned long >::const_iterator it = teams.begin();  it != teams.end(); ++it )
        {
            kernel::Team_ABC* pTeam = model.FindTeam( *it );
            if( pTeam )
                InsertAutomats( automats, *pTeam, model );
        }
    }

    void InsertAutomatsFromFormations( const std::vector< unsigned long >& formations, std::set< unsigned long >& automats, const Model& model )
    {
        for( std::vector< unsigned long >::const_iterator it = formations.begin();  it != formations.end(); ++it )
        {
            kernel::Formation_ABC* pFormation = model.FindFormation( *it );
            if( pFormation )
                InsertAutomats( automats, *pFormation, model );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfile::VisitAllAutomats
// Created: MMC 2012-04-23
// -----------------------------------------------------------------------------
void UserProfile::VisitAllAutomats( std::set< unsigned long >& elements ) const
{
    elements.insert( readAutomats_.begin(), readAutomats_.end() );
    elements.insert( writeAutomats_.begin(), writeAutomats_.end() );
    InsertAutomatsFromTeams( readSides_, elements, model_ );
    InsertAutomatsFromTeams( writeSides_, elements, model_ );
    InsertAutomatsFromFormations( readFormations_, elements, model_ );
    InsertAutomatsFromFormations( writeFormations_, elements, model_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetWriteProfilesCount
// Created: LGY 2012-03-22
// -----------------------------------------------------------------------------
unsigned int UserProfile::GetWriteProfilesCount()
{
    return static_cast< unsigned int >( writeSides_.size() + writeFormations_.size() + writeAutomats_.size() + writePopulations_.size() + writeGhosts_.size() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetProfilesCount
// Created: LGY 2012-02-07
// -----------------------------------------------------------------------------
unsigned int UserProfile::GetProfilesCount() const
{
    return static_cast< unsigned int >( writeSides_.size() + writeFormations_.size() + writeAutomats_.size() + writePopulations_.size() + writeGhosts_.size() +
                                        readSides_.size()  + readFormations_.size() + readAutomats_.size() + readPopulations_.size() + readGhosts_.size() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetAutomatAndPopulationsProfilesCount
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
unsigned int UserProfile::GetAutomatAndPopulationsProfilesCount() const
{
    std::set< unsigned long > elements;
    VisitAllAutomats( elements );
    return static_cast< unsigned int >( elements.size() + writePopulations_.size() + readPopulations_.size() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::ComputeLowLevel
// Created: LDC 2012-04-30
// -----------------------------------------------------------------------------
void UserProfile::ComputeLowLevel()
{
    isLowLevel_ = ( userRole_ == "anibas" || userRole_ == "eniex" || userRole_ == "environment" );
}
