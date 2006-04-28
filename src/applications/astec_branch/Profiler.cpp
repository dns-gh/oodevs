// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Profiler.h"

// -----------------------------------------------------------------------------
// Name: Profiler constructor
// Created: AGE 2006-04-26
// -----------------------------------------------------------------------------
Profiler::Profiler( const std::string& name /*= ""*/, std::ostream& out /*= std::cout*/ )
    : bDenis_( InitializeProfiler() )
    , name_( name )
    , out_( &out )
{
    profiler_.Start();
}

// -----------------------------------------------------------------------------
// Name: Profiler destructor
// Created: AGE 2006-04-26
// -----------------------------------------------------------------------------
Profiler::~Profiler()
{
    profiler_.Stop();
    *out_ << "Profiled " << name_ << " : " << profiler_.GetLastTime() << " ms" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Profiler::InitializeProfiler
// Created: AGE 2006-04-26
// -----------------------------------------------------------------------------
bool Profiler::InitializeProfiler()
{
    static bool initialized = false;
    if( ! initialized )
        MT_Profiler::Initialize();
    return initialized = true;;
}

