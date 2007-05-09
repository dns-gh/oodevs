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
// Name: Population::GetID
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
unsigned long Population::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< PopulationConcentration >& Population::GetConcentrations
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
inline
const ModelsContainer< PopulationConcentration >& Population::GetConcentrations() const
{
    return concentrations_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< PopulationFlow          >& Population::GetFlows
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
inline
const ModelsContainer< PopulationFlow          >& Population::GetFlows() const
{
    return flows_;
}

}
