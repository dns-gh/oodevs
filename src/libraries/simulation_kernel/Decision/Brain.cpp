// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "Brain.h"
#include "MT_Tools/MT_Logger.h"

using namespace sword;

Brain::T_Profilers Brain::profilers_;

// -----------------------------------------------------------------------------
// Name: Brain constructor
// Created: MCO 2012-10-31
// -----------------------------------------------------------------------------
Brain::Brain( const std::string& config, DEC_Logger* logger )
    : logger_( logger )
    , brain_ ( new directia::brain::Brain( config ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Brain constructor
// Created: MCO 2012-10-31
// -----------------------------------------------------------------------------
Brain::Brain( Brain& parent, DEC_Logger* logger )
    : logger_( logger )
    , brain_ ( new directia::brain::Brain( *parent.brain_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Brain::SelectActions
// Created: MCO 2012-10-31
// -----------------------------------------------------------------------------
void Brain::SelectActions()
{
    brain_->SelectActions();
}

// -----------------------------------------------------------------------------
// Name: Brain::TriggerSelectedActions
// Created: MCO 2012-10-31
// -----------------------------------------------------------------------------
void Brain::TriggerSelectedActions( double elapsedTime /*= 1*/ ) const
{
    brain_->TriggerSelectedActions( elapsedTime );
}

void Brain::ExecuteScript( const std::string& script )
{
    brain_->Execute( script );
}

// -----------------------------------------------------------------------------
// Name: Brain::GetScriptRef
// Created: MCO 2012-10-31
// -----------------------------------------------------------------------------
directia::tools::binders::ScriptRef Brain::GetScriptRef( const std::string& s )
{
    return (*brain_)[ s ];
}

// -----------------------------------------------------------------------------
// Name: Brain::GetScriptRef
// Created: MCO 2012-11-02
// -----------------------------------------------------------------------------
directia::tools::binders::ScriptRef Brain::GetScriptRef()
{
    return *brain_;
}

// -----------------------------------------------------------------------------
// Name: Brain::ResetProfiling
// Created: MCO 2012-11-05
// -----------------------------------------------------------------------------
void Brain::ResetProfiling( bool log )
{
    if( log )
        for( T_Profilers::const_iterator it = profilers_.begin(); it != profilers_.end(); ++it )
            if( it->second.GetCount() != 0 )
                MT_LOG_INFO_MSG( "<profiling> DEC " << it->first << " " << it->second.GetCount() << " " << it->second.GetTotalTime() << " ms" );
    for( T_Profilers::iterator it = profilers_.begin(); it != profilers_.end(); ++it )
        it->second.Reset();
}

// -----------------------------------------------------------------------------
// Name: Brain::GetTotalTime
// Created: MCO 2012-11-06
// -----------------------------------------------------------------------------
double Brain::GetTotalTime()
{
    double result = 0;
    for( T_Profilers::const_iterator it = profilers_.begin(); it != profilers_.end(); ++it )
        result += it->second.GetTotalTime();
    return result;
}
