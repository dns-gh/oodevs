// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_TypeComposante.inl $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: MOS_TypeComposante.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------// Name: MOS_TypeComposante::GetName
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const std::string& MOS_TypeComposante::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: MOS_TypeComposante::GetID
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
uint MOS_TypeComposante::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypeComposante::IsLogistic
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool MOS_TypeComposante::IsLogistic() const
{
    return HasMaintenance() || HasMedical() || HasSupply();
}

// -----------------------------------------------------------------------------
// Name: MOS_TypeComposante::HasMaintenance
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool MOS_TypeComposante::HasMaintenance() const
{
    return bHasMaintenance_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypeComposante::HasMedical
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool MOS_TypeComposante::HasMedical() const
{
    return bHasMedical_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypeComposante::HasSupply
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool MOS_TypeComposante::HasSupply() const
{
    return bHasSupply_;
}
