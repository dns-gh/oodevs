// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Decision.h"
#include "MIL_AgentServer.h"
#include "MT_Tools/MT_CrashHandler.h"

// -----------------------------------------------------------------------------
// Name: DEC_Decision constructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
DEC_Decision::DEC_Decision( MIL_Entity_ABC& entity, const std::string& type )
: DEC_Decision_ABC( entity )
, DIA_Engine      ( *DIA_TypeManager::Instance().GetType( type ), "" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision constructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
DEC_Decision::DEC_Decision( const std::string& type )
: DIA_Engine      ( *DIA_TypeManager::Instance().GetType( type ), "" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision destructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
DEC_Decision::~DEC_Decision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::UpdateDecision
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
void DEC_Decision::UpdateDecision()
{
    __try
    {
        PrepareUpdate    ();
        UpdateMotivations( (float)MIL_AgentServer::GetWorkspace().GetTimeStepDuration() );
        UpdateDecisions  ();
        ExecuteAllActions();
    }
    __except( MT_CrashHandler::ExecuteHandler( GetExceptionInformation() ) )
    {
        HandleUpdateDecisionError();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::Reset
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
void DEC_Decision::Reset()
{

}
