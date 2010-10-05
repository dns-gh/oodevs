// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Exercises.h"
#include "Exercise.h"
#include "clients_kernel/Controller.h"
#include "protocol/Launcher.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Exercises constructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
Exercises::Exercises( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Exercises destructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
Exercises::~Exercises()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Exercises::Update
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
void Exercises::Update( const MsgsLauncherToAdmin::MsgExercicesListResponse& message )
{
    DeleteAll();
    for( int i = 0; i < message.exercise().size(); ++i )
    {
        Exercise* exercise = new Exercise( message.exercise( i ) );
        Register( exercise->GetName(), *exercise );
    }
    controller_.Update( *this );
}
