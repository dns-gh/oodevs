// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::HasFunction
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
bool MIL_LimaOrder::HasFunction( const MIL_LimaFunction& function ) const
{
    return functions_.find( &function ) != functions_.end();
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::GetPoints
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
const T_PointVector& MIL_LimaOrder::GetPoints() const
{
    return points_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::GetSchedule
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
inline
uint MIL_LimaOrder::GetSchedule() const
{
    return nSchedule_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::FlagSchedule
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
inline
void MIL_LimaOrder::FlagSchedule( bool bFlag )
{
    bScheduleFlag_ = bFlag;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::IsScheduleFlagged
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
inline
bool MIL_LimaOrder::IsScheduleFlagged() const
{
    return bScheduleFlag_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::Flag
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
void MIL_LimaOrder::Flag( bool bFlag )
{
    bFlag_ = bFlag;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::IsFlagged
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
bool MIL_LimaOrder::IsFlagged() const
{
    return bFlag_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::GetID
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
uint MIL_LimaOrder::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::GetFunctions
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
const MIL_LimaOrder::T_LimaFunctions& MIL_LimaOrder::GetFunctions() const
{
    return functions_;
}
