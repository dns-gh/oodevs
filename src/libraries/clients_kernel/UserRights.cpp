// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "UserRights.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserRights constructor
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
UserRights::UserRights()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserRights destructor
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
UserRights::~UserRights()
{
        // NOTHING
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
// Name: UserRights::IsReadable
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
bool UserRights::IsReadable( const kernel::Entity_ABC& entity ) const
{
    return FindIn( entity.GetId(), readSides_ )
        || FindIn( entity.GetId(), readFormations_ )
        || FindIn( entity.GetId(), readAutomats_ )
        || FindIn( entity.GetId(), readPopulations_ );
}

// -----------------------------------------------------------------------------
// Name: UserRights::IsWriteable
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
bool UserRights::IsWriteable( const kernel::Entity_ABC& entity ) const
{
    return FindIn( entity.GetId(), writeSides_ )
        || FindIn( entity.GetId(), writeFormations_ )
        || FindIn( entity.GetId(), writeAutomats_ )
        || FindIn( entity.GetId(), writePopulations_ );
}

namespace
{
    void SetRight( unsigned long id, std::vector< unsigned long >& ids, bool status )
    {
        std::vector< unsigned long >::iterator it = std::find( ids.begin(), ids.end(), id );
        if( it == ids.end() && status )
            ids.push_back( id );
        else if( it != ids.end() && !status )
            ids.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: UserRights::SetReadable
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserRights::SetReadable( const kernel::Entity_ABC& entity, bool readable )
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
// Name: UserRights::SetWriteable
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserRights::SetWriteable( const kernel::Entity_ABC& entity, bool writeable )
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

// -----------------------------------------------------------------------------
// Name: UserRights::FindSide
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
bool UserRights::FindSide( unsigned long id ) const
{
    return( std::find( readSides_.begin(), readSides_.end(), id ) != readSides_.end() 
          || std::find( writeSides_.begin(), writeSides_.end(), id ) != writeSides_.end() );
}

// -----------------------------------------------------------------------------
// Name: UserRights::Clear
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
void UserRights::Clear()
{
    readSides_.clear();
    writeSides_.clear();
    readAutomats_.clear();
    writeAutomats_.clear();
    readPopulations_.clear();
    writePopulations_.clear();
    readFormations_.clear();
    writeFormations_.clear();
}

// -----------------------------------------------------------------------------
// Name: UserRights::Read
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
void UserRights::Read( xml::xistream& xis, const ExistenceChecker_ABC& teamChecker, const ExistenceChecker_ABC& formationChecker, const ExistenceChecker_ABC& automatChecker, const ExistenceChecker_ABC& populationChecker )
{
    xis >> xml::start( "readonly" )
            >> xml::list( "side", *this, &UserRights::ReadRights, readSides_, teamChecker )
            >> xml::list( "formation", *this, &UserRights::ReadRights, readFormations_, formationChecker )
            >> xml::list( "automat", *this, &UserRights::ReadRights, readAutomats_, automatChecker )
            >> xml::list( "crowd", *this, &UserRights::ReadRights, readPopulations_, populationChecker )
        >> xml::end
        >> xml::start( "readwrite" )
            >> xml::list( "side", *this, &UserRights::ReadRights, writeSides_, teamChecker )
            >> xml::list( "formation", *this, &UserRights::ReadRights, writeFormations_, formationChecker )
            >> xml::list( "automat", *this, &UserRights::ReadRights, writeAutomats_, automatChecker )
            >> xml::list( "crowd", *this, &UserRights::ReadRights, writePopulations_, populationChecker )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: UserRights::ReadRights
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
void UserRights::ReadRights( xml::xistream& xis, T_Ids& list, const ExistenceChecker_ABC& checker )
{
    unsigned long id;
    xis >> xml::attribute( "id", id );
    if( checker( id ) )
        list.push_back( id );
    else
        std::cerr << "Invalid entity id in profile: " << id << std::endl; // $$$$ SBO 2007-11-06:
}

// -----------------------------------------------------------------------------
// Name: UserRights::SerializeRights
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
void UserRights::SerializeRights( xml::xostream& xos, const std::string& tag, const T_Ids& list ) const
{
    for( T_Ids::const_iterator it = list.begin(); it != list.end(); ++it )
        xos << xml::start( tag )
                << xml::attribute( "id", *it )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UserRights::Serialize
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
void UserRights::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "readonly" );
    SerializeRights( xos, "side", readSides_ );
    SerializeRights( xos, "formation", readFormations_ );
    SerializeRights( xos, "automat", readAutomats_ );
    SerializeRights( xos, "crowd", readPopulations_ );
    xos << xml::end
        << xml::start( "readwrite" );
    SerializeRights( xos, "side", writeSides_ );
    SerializeRights( xos, "formation", writeFormations_ );
    SerializeRights( xos, "automat", writeAutomats_ );
    SerializeRights( xos, "crowd", writePopulations_ );
    xos << xml::end;
}


// -----------------------------------------------------------------------------
// Name: UserRights::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserRights::NotifyTeamDeleted( unsigned long teamId )
{
    SetRight( teamId, readSides_, false );
    SetRight( teamId, writeSides_, false );
}

// -----------------------------------------------------------------------------
// Name: UserRights::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserRights::NotifyFormationDeleted( unsigned long formationId )
{
    SetRight( formationId, readFormations_, false );
    SetRight( formationId, writeFormations_, false );
}

// -----------------------------------------------------------------------------
// Name: UserRights::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserRights::NotifyAutomatDeleted( unsigned long automatId )
{
    SetRight( automatId, readAutomats_, false );
    SetRight( automatId, writeAutomats_, false );
}

// -----------------------------------------------------------------------------
// Name: UserRights::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserRights::NotifyPopulationDeleted( unsigned long populationId )
{
    SetRight( populationId, readPopulations_, false );
    SetRight( populationId, writePopulations_, false );
}

// -----------------------------------------------------------------------------
// Name: UserRights::InsertWriteSides
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
void UserRights::InsertWriteSides( T_Ids& elements ) const
{
    elements.insert( elements.begin(), writeSides_.begin(), writeSides_.end() );
}
// -----------------------------------------------------------------------------
// Name: UserRights::InsertWriteAutomats
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
void UserRights::InsertWriteAutomats( T_Ids& elements ) const
{
    elements.insert( elements.begin(), writeAutomats_.begin(), writeAutomats_.end() );
}
// -----------------------------------------------------------------------------
// Name: UserRights::InsertWritePopulations
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
void UserRights::InsertWritePopulations( T_Ids& elements ) const
{
    elements.insert( elements.begin(), writePopulations_.begin(), writePopulations_.end() );
}

// -----------------------------------------------------------------------------
// Name: UserRights::GetWriteSides
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
const UserRights::T_Ids& UserRights::GetWriteSides() const
{
    return writeSides_;
}

// -----------------------------------------------------------------------------
// Name: UserRights::GetWriteAutomats
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
const UserRights::T_Ids& UserRights::GetWriteAutomats() const
{
    return writeAutomats_;
}

// -----------------------------------------------------------------------------
// Name: UserRights::GetWritePopulations
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
const UserRights::T_Ids& UserRights::GetWritePopulations() const
{
    return writePopulations_;
}

// -----------------------------------------------------------------------------
// Name: UserRights::GetWriteFormations
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
const UserRights::T_Ids& UserRights::GetWriteFormations() const
{
    return writeFormations_;
}

// -----------------------------------------------------------------------------
// Name: UserRights::GetReadSides
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
const UserRights::T_Ids& UserRights::GetReadSides() const
{
    return readSides_;
}

// -----------------------------------------------------------------------------
// Name: UserRights::GetReadAutomats
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
const UserRights::T_Ids& UserRights::GetReadAutomats() const
{
    return readAutomats_;
}

// -----------------------------------------------------------------------------
// Name: UserRights::GetReadPopulations
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
const UserRights::T_Ids& UserRights::GetReadPopulations() const
{
    return readPopulations_;
}

// -----------------------------------------------------------------------------
// Name: UserRights::GetReadFormations
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
const UserRights::T_Ids& UserRights::GetReadFormations() const
{
    return readFormations_;
}
