// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PopulationType.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 3 $
// $Workfile: PopulationType.inl $
//
// *****************************************************************************

namespace TEST
{

// -----------------------------------------------------------------------------
// Name: PopulationType::GetName
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string& PopulationType::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: PopulationType::GetId
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
uint PopulationType::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: PopulationType::GetModel
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
const PopulationModel& PopulationType::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

} // end namespace TEST
