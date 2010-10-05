// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Exercise.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Exercise constructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
Exercise::Exercise( const Common::MsgExercise& message )
    : name_( message.name() )
    , port_( message.has_port() ? message.port() : 0 )
    , running_( message.has_running() ? message.running() : false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Exercise destructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
Exercise::~Exercise()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Exercise::GetName
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
std::string Exercise::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Exercise::GetPort
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
unsigned int Exercise::GetPort() const
{
    return port_;
}

// -----------------------------------------------------------------------------
// Name: Exercise::IsRunning
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
bool Exercise::IsRunning() const
{
    return running_;
}
