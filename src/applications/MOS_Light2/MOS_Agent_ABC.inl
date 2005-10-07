// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************



// -----------------------------------------------------------------------------
// Name: MOS_Agent_ABC::GetReports
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
MOS_Agent_ABC::T_ReportVector& MOS_Agent_ABC::GetReports()
{
    return reportVector_;
}



// -----------------------------------------------------------------------------
// Name: MOS_Agent_ABC::FindAgentKnowledge
// Created: HME 2005-10-07
// -----------------------------------------------------------------------------
inline
MOS_AgentKnowledge* MOS_Agent_ABC::FindAgentKnowledge( uint nId )
{
    return agentKnowledge_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent_ABC::GetFireResult
// Created: HME 2005-10-07
// -----------------------------------------------------------------------------
inline
T_FireResults&       MOS_Agent_ABC::GetFireResults()
{
    return fireResults_;
}