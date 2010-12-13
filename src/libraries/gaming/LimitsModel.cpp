// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LimitsModel.h"
#include "TacticalLine_ABC.h"
#include "TacticalLineFactory.h"

// -----------------------------------------------------------------------------
// Name: LimitsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::LimitsModel( TacticalLineFactory& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimitsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::~LimitsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Purge
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
void LimitsModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Create
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::Create( const sword::LimitCreation& message )
{
    TacticalLine_ABC* line = factory_.Create( message );
    Register( message.id().id(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Create
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::Create( const sword::LimaCreation& message )
{
    TacticalLine_ABC* line = factory_.Create( message );
    Register( message.id().id(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::DeleteLimit
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::DeleteLimit( unsigned long id )
{
    TacticalLine_ABC* line = Find( id );
    Remove( id );
    delete line;
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::DeleteLima
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::DeleteLima( unsigned long id )
{
    TacticalLine_ABC* line = Find( id );
    Remove( id );
    delete line;
}
