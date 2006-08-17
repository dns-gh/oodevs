// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

namespace TEST {

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetId
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
inline
T_EntityId PopulationKnowledge::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetRealPopulation
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
inline
const Population* PopulationKnowledge::GetRealPopulation() const
{
    return pRealPopulation_;
}

} // end namespace TEST