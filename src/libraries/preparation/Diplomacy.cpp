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

Diplomacy Diplomacy::unknown_( tools::translate( "Preparation", "inconnu" ) );
Diplomacy Diplomacy::friend_ ( tools::translate( "Preparation", "ami"     ) );
Diplomacy Diplomacy::enemy_  ( tools::translate( "Preparation", "ennemi"  ) );
Diplomacy Diplomacy::neutral_( tools::translate( "Preparation", "neutre"  ) );

// -----------------------------------------------------------------------------
// Name: Diplomacy constructor
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy::Diplomacy()
    : name_( unknown_.name_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Diplomacy constructor
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy::Diplomacy( const QString& name )
    : name_( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Diplomacy constructor
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy::Diplomacy( const Diplomacy& diplomacy )
    : name_( diplomacy.name_ )
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
// Name: Diplomacy::Unknown
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
Diplomacy Diplomacy::Unknown()
{
    return unknown_;
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
    name_ = rhs.name_;
    return *this;
}
