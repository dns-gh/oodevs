// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ScoreModel.h"
#include "Score.h"

// -----------------------------------------------------------------------------
// Name: ScoreModel constructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
ScoreModel::ScoreModel( kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreModel destructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
ScoreModel::~ScoreModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ScoreModel::Purge
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScoreModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ScoreModel::Update
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScoreModel::Update( const ASN1T_MsgIndicator& message )
{
    Score* score = Find( message.name );
    if( !score )
    {
        score = new Score( message, controller_, publisher_ );
        Register( score->GetName(), *score );
    }
    score->Update( message );
}
