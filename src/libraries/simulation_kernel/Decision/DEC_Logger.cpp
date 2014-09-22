// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Logger.h"
#include "MT_Tools/MT_Logger.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: DEC_Logger constructor
// Created: SLI 2013-02-22
// -----------------------------------------------------------------------------
DEC_Logger::DEC_Logger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Logger constructor
// Created: SLI 2013-02-22
// -----------------------------------------------------------------------------
DEC_Logger::DEC_Logger( const std::string& type )
    : type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Logger destructor
// Created: SLI 2013-02-22
// -----------------------------------------------------------------------------
DEC_Logger::~DEC_Logger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Logger::Log
// Created: SLI 2013-02-22
// -----------------------------------------------------------------------------
void DEC_Logger::Log( const char* name, unsigned int identifier )
{
    MT_LOG_INFO_MSG( "DEC_CALL " << type_ << " " << identifier << " " << name );
}
