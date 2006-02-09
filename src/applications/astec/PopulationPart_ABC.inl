// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC::GetLivingHumans
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
uint PopulationPart_ABC::GetLivingHumans() const
{
    return nLivingHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC::GetDeadHumans
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
uint PopulationPart_ABC::GetDeadHumans() const
{
    return nDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC::GetID
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
uint PopulationPart_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC::GetStringAttitude
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const std::string& PopulationPart_ABC::GetStringAttitude() const
{
    return ENT_Tr::ConvertFromPopulationAttitude( attitude_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC::GetAttitude
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
inline
E_PopulationAttitude PopulationPart_ABC::GetAttitude() const
{
    return  attitude_ ;
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC::GetDensity
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
inline
MT_Float PopulationPart_ABC::GetDensity() const
{
    return rDensity_;
}
