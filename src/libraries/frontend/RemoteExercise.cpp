// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "RemoteExercise.h"
#include "Host_ABC.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"
#include "protocol/LauncherSenders.h"
#include <boost/algorithm/string.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: RemoteExercise constructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
RemoteExercise::RemoteExercise( const tools::Path& exercise, kernel::Controller& controller )
    : controller_( controller )
    , name_      ( exercise )
{
    controller_.Create( static_cast< Exercise_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise destructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
RemoteExercise::~RemoteExercise()
{
    controller_.Delete( static_cast< Exercise_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::GetName
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
const tools::Path& RemoteExercise::GetName() const
{
    return name_;
}

