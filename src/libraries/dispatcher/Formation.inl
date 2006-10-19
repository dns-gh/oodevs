// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace dispatcher
{

    // -----------------------------------------------------------------------------
// Name: ModelRefsContainer< Formation >& Formation::GetSubordinates
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
inline
ModelRefsContainer< Formation >& Formation::GetSubordinates()
{
    return subordinates_;
}

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer< Automat >& Formation::GetAutomats
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
inline
ModelRefsContainer< Automat >& Formation::GetAutomats()
{
    return automats_;
}

// -----------------------------------------------------------------------------
// Name: Formation::GetID
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
inline
unsigned long Formation::GetID() const
{
    return nID_;
}

}
