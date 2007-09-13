// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Objective::ComputerBarycenter
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
inline
MT_Vector2D DEC_Objective::ComputerBarycenter() const
{   
    return localisation_.ComputeBarycenter();
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::GetSchedule
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
inline
uint DEC_Objective::GetSchedule() const
{
    return nSchedule_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::IsFlagged
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
inline
bool DEC_Objective::IsFlagged() const
{
    return bFlag_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::Flag
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
inline
void DEC_Objective::Flag( bool bValue )
{
    bFlag_ = bValue;
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::GetLocalisation
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
inline
const TER_Localisation& DEC_Objective::GetLocalisation() const
{
    return localisation_;
}
