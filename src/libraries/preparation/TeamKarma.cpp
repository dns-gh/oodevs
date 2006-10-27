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
TeamKarma::TeamKarma( const QString& name, const QString& value )
    : name_( name )
    , value_( value )
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
