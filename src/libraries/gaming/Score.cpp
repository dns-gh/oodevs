// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Score.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
Score::Score( const ASN1T_MsgIndicator& message, kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher )
    , name_( message.name )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Score destructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
Score::~Score()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: Score::GetName
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
QString Score::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Score::Update
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void Score::Update( const ASN1T_MsgIndicator& message )
{
    value_ = message.value;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Score::Display
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void Score::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Score", "Name" ), name_ )
             .Display( tools::translate( "Score", "Value" ), value_ );
}
