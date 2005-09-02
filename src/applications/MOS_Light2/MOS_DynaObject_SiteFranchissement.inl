// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_SiteFranchissement::GetLargeur
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject_SiteFranchissement::GetLargeur() const
{
    return nLargeur_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_SiteFranchissement::GetProfondeur
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject_SiteFranchissement::GetProfondeur() const
{
    return nProfondeur_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_SiteFranchissement::GetVitesseCourant
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject_SiteFranchissement::GetVitesseCourant() const
{
    return nVitesseCourant_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_SiteFranchissement::GetBergesAAmenager
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
bool MOS_DynaObject_SiteFranchissement::AreBergesAAmenager() const
{
    return bBergesAAmenager_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_SiteFranchissement::SetParameters
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
void MOS_DynaObject_SiteFranchissement::SetParameters( uint nWidth, uint nDepth, uint nFlow, bool bConstruct )
{
    nLargeur_          = nWidth;
    nProfondeur_       = nDepth;
    nVitesseCourant_   = nFlow;
    bBergesAAmenager_  = bConstruct;
}
