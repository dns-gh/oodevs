// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Karma.h"
#include "Tools.h"
#include "Team_ABC.h"

using namespace kernel;

const Karma Karma::friend_ ( 1, "friend" );  // f
const Karma Karma::enemy_  ( 2, "enemy" );   // h
const Karma Karma::neutral_( 3, "neutral" ); // n
const Karma Karma::unknown_( 0, "unknown" ); // u

// -----------------------------------------------------------------------------
// Name: Karma constructor
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
Karma::Karma()
    : uId_( unknown_.uId_ )
    , identifier_( unknown_.identifier_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Karma constructor
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
Karma::Karma( const unsigned int id, const std::string& identifier )
    : uId_( id )
    , identifier_( identifier )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Karma destructor
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
Karma::~Karma()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Karma::GetId
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
const std::string& Karma::GetId() const
{
    return identifier_;
}
// -----------------------------------------------------------------------------
// Name: Karma::GetUid
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
unsigned int Karma::GetUId() const
{
    return uId_;
}

// -----------------------------------------------------------------------------
// Name: Karma::GetName
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
QString Karma::GetName() const
{
    switch( uId_ )
    {
    case 1:
        return tools::translate( "Karma", "Friend" );
    case 2:
        return tools::translate( "Karma", "Enemy" );
    case 3:
        return tools::translate( "Karma", "Neutral" );
    default:
        return tools::translate( "Karma", "Unknown" );
    }
}

// -----------------------------------------------------------------------------
// Name: Karma::operator!
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
const Karma& Karma::operator!() const
{
    if( *this == friend_ )
        return enemy_;
    if( *this == enemy_ )
        return friend_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Karma::operator<
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
bool Karma::operator<( const Karma& rhs ) const
{
    return this->GetName().compare( rhs.GetName() ) < 0;
}

// -----------------------------------------------------------------------------
// Name: Karma::operator==
// Created: SBO 2007-03-08
// -----------------------------------------------------------------------------
bool Karma::operator==( const Karma& rhs ) const
{
    return uId_ == rhs.uId_;
}

// -----------------------------------------------------------------------------
// Name: Karma::ResolveId
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
const Karma& Karma::ResolveId( const std::string& id )
{
    if( id == friend_.GetId() )
        return friend_;
    if( id == enemy_.GetId() )
        return enemy_;
    if( id == neutral_.GetId() )
        return neutral_;
    return unknown_;
}

// -----------------------------------------------------------------------------
// Name: Karma::ResolveName
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
const Karma& Karma::ResolveName( const QString& name )
{
    if( name == friend_.GetName() )
        return friend_;
    if( name == enemy_.GetName() )
        return enemy_;
    if( name == neutral_.GetName() )
        return neutral_;
    return unknown_;
}

// -----------------------------------------------------------------------------
// Name: Karma::RelativeTo
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
const Karma& Karma::RelativeTo( const Karma& rhs ) const
{
    if( *this == rhs )
        return friend_;
    if( *this == !rhs )
        return enemy_;
    if( *this == unknown_ || rhs == unknown_ )
        return unknown_;
    return neutral_;
}
