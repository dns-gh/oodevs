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

Karma Karma::friend_ ( "friend" , tools::translate( "Karma", "Friend" ) );  // f
Karma Karma::enemy_  ( "enemy"  , tools::translate( "Karma", "Enemy" ) );   // h
Karma Karma::neutral_( "neutral", tools::translate( "Karma", "Neutral" ) ); // n
Karma Karma::unknown_( "unknown", tools::translate( "Karma", "Unknown" ) ); // u

// -----------------------------------------------------------------------------
// Name: Karma constructor
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
Karma::Karma()
    : identifier_( unknown_.identifier_ )
    , name_( unknown_.name_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Karma constructor
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
Karma::Karma( const std::string& identifier, const QString& name )
    : identifier_( identifier )
    , name_( name )
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
std::string Karma::GetId() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: Karma::GetName
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
QString Karma::GetName() const
{
    return name_;
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
    return identifier_ < rhs.identifier_ || ( identifier_ >= rhs.identifier_ && name_ < rhs.name_ );
}

// -----------------------------------------------------------------------------
// Name: Karma::operator==
// Created: SBO 2007-03-08
// -----------------------------------------------------------------------------
bool Karma::operator==( const Karma& rhs ) const
{
    return identifier_ == rhs.identifier_;
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
