// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Diplomacy.h"
#include "clients_gui/Tools.h"

Diplomacy Diplomacy::friend_ ( "friend" , tools::translate( "Preparation", "ami"     ) );
Diplomacy Diplomacy::enemy_  ( "enemy"  , tools::translate( "Preparation", "ennemi"  ) );
Diplomacy Diplomacy::neutral_( "neutral", tools::translate( "Preparation", "neutre"  ) );

// -----------------------------------------------------------------------------
// Name: Diplomacy constructor
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy::Diplomacy()
    : value_( neutral_.value_ )
    , name_( neutral_.name_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Diplomacy constructor
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy::Diplomacy( const QString& value, const QString& name )
    : value_( value )
    , name_( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Diplomacy constructor
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy::Diplomacy( const Diplomacy& diplomacy )
    : value_( diplomacy.value_ )
    , name_( diplomacy.name_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Diplomacy destructor
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy::~Diplomacy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Diplomacy::GetName
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
QString Diplomacy::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Diplomacy::GetValue
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
QString Diplomacy::GetValue() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: Diplomacy::Friend
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy Diplomacy::Friend()
{
    return friend_;
}

// -----------------------------------------------------------------------------
// Name: Diplomacy::Enemy
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy Diplomacy::Enemy()
{
    return enemy_;
}

// -----------------------------------------------------------------------------
// Name: Diplomacy::Neutral
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy Diplomacy::Neutral()
{
    return neutral_;
}

// -----------------------------------------------------------------------------
// Name: Diplomacy::operator=
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy& Diplomacy::operator=( const Diplomacy& rhs )
{
    value_ = rhs.value_;
    name_  = rhs.name_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Diplomacy::Resolve
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
Diplomacy Diplomacy::Resolve( const QString& value )
{
    if( value == friend_.value_ )
        return friend_;
    if( value == enemy_.value_ )
        return enemy_;
    return neutral_;
}
