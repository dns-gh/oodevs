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
#include "Model.h"
#include "AgentsModel.h"
#include "TeamsModel.h"
#include "FormationModel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::UserProfile( xml::xistream& xis, kernel::Controller& controller, const Model& model )
    : controller_( controller )
    , model_( model )
    , supervisor_( false )
{
    const ExistenceChecker< kernel::Resolver< kernel::Team_ABC > >       teamChecker( model_.teams_ );
    const ExistenceChecker< kernel::Resolver< kernel::Formation_ABC > >  formationChecker( model_.formations_ );
    const ExistenceChecker< kernel::Resolver< kernel::Automat_ABC > >    automatChecker( model_.agents_ );
    const ExistenceChecker< kernel::Resolver< kernel::Population_ABC > > populationChecker( model_.agents_ );

    std::string login, pass;
    xis >> attribute( "name", login )
        >> attribute( "password", pass )
        >> xml::optional() >> content( "supervision", supervisor_ )
        >> start( "rights" )
            >> start( "readonly" )
                >> list( "side"      , *this, &UserProfile::ReadRights, readSides_, teamChecker )
                >> list( "formation" , *this, &UserProfile::ReadRights, readFormations_, formationChecker )
                >> list( "automat"   , *this, &UserProfile::ReadRights, readAutomats_, automatChecker )
                >> list( "population", *this, &UserProfile::ReadRights, readPopulations_, populationChecker )
            >> end()
            >> start( "readwrite" )
                >> list( "side"      , *this, &UserProfile::ReadRights, writeSides_, teamChecker )
                >> list( "formation" , *this, &UserProfile::ReadRights, writeFormations_, formationChecker )
                >> list( "automat"   , *this, &UserProfile::ReadRights, writeAutomats_, automatChecker )
                >> list( "population", *this, &UserProfile::ReadRights, writePopulations_, populationChecker )
            >> end()
        >> end();
    login_ = login.c_str();
    password_ = pass.c_str();
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const QString& login, kernel::Controller& controller, const Model& model )
    : controller_( controller )
    , model_     ( model )
    , login_     ( login )
    , supervisor_( false )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::~UserProfile()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::Serialize
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfile::Serialize( xml::xostream& xos ) const
{
    xos << start( "profile" )
            << attribute( "name", login_.ascii() )
            << attribute( "password", password_.ascii() )
            << start( "rights" )
                << start( "readonly" );
    SerializeRights( xos, "side", readSides_ );
    SerializeRights( xos, "formation", readFormations_ );
    SerializeRights( xos, "automat", readAutomats_ );
    SerializeRights( xos, "population", readPopulations_ );
    xos         << end()
                << start( "readwrite" );
    SerializeRights( xos, "side", writeSides_ );
    SerializeRights( xos, "formation", writeFormations_ );
    SerializeRights( xos, "automat", writeAutomats_ );
    SerializeRights( xos, "population", writePopulations_ );
    xos         << end()
                << content( "supervision", supervisor_ )
            << end()
        << end();
}

// -----------------------------------------------------------------------------
// Name: UserProfile::ReadRights
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::ReadRights( xml::xistream& xis, T_Ids& list, const ExistenceChecker_ABC& checker )
{
    unsigned long id;
    xis >> attribute( "id", id );
    if( checker( id ) )
        list.push_back( id );
    else
        std::cout << "Invalid entity id in profile: " << id << std::endl;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SerializeRights
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfile::SerializeRights( xml::xostream& xos, const std::string& tag, const T_Ids& list ) const
{
    for( T_Ids::const_iterator it = list.begin(); it != list.end(); ++it )
        xos << start( tag )
                << attribute( "id", *it )
            << end();
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
// Name: UserProfile::IsSupervisor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
bool UserProfile::IsSupervisor() const
{
    return supervisor_;
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
        || FindIn( entity.GetId(), readPopulations_ );
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
        || FindIn( entity.GetId(), writePopulations_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetLogin
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetLogin( const QString& value )
{
    login_ = value;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetPassword
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetPassword( const QString& value )
{
    password_ = value;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetSupervisor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetSupervisor( bool value )
{
    supervisor_ = value;
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
}
