// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamKarma.h"

// -----------------------------------------------------------------------------
// Name: TeamKarma constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
TeamKarma::TeamKarma( const QString& name, const QString& value, char id )
    : name_( name )
    , value_( value )
    , id_( id  )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TeamKarma destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
TeamKarma::~TeamKarma()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamKarma::GetName
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
const QString& TeamKarma::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: TeamKarma::GetValue
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
const QString& TeamKarma::GetValue() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: TeamKarma::GetIdentifier
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
char TeamKarma::GetIdentifier() const
{
    return id_;
}
