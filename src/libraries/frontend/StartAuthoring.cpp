// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "StartAuthoring.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: StartAuthoring constructor
// Created: JSR 2010-06-10
// -----------------------------------------------------------------------------
StartAuthoring::StartAuthoring( const tools::GeneralConfig& config )
    : SpawnCommand( config, MakeBinaryName( "adaptation_app" ), "adaptation" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartAuthoring destructor
// Created: JSR 2010-06-10
// -----------------------------------------------------------------------------
StartAuthoring::~StartAuthoring()
{
    // NOTHING
}
