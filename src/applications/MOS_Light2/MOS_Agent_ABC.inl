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
// Name: MOS_Agent_ABC::GetFireResult
// Created: HME 2005-10-19
// -----------------------------------------------------------------------------
inline
const T_FireResults& MOS_Agent_ABC::GetFireResult() const
{
    return fireResults_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent_ABC::GetDebugDrawPoints
// Created: SBO 2005-12-06
// -----------------------------------------------------------------------------
inline
const std::vector< MT_Vector2D >& MOS_Agent_ABC::GetDebugDrawPoints() const
{
    return reportPoints_;
}
