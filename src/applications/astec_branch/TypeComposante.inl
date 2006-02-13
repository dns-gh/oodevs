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
// $Archive: /MVW_v10/Build/SDK/Light2/src/TypeComposante.inl $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: TypeComposante.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------// Name: TypeComposante::GetName
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const std::string& TypeComposante::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: TypeComposante::GetID
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
uint TypeComposante::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: TypeComposante::IsLogistic
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool TypeComposante::IsLogistic() const
{
    return HasMaintenance() || HasMedical() || HasSupply();
}

// -----------------------------------------------------------------------------
// Name: TypeComposante::HasMaintenance
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool TypeComposante::HasMaintenance() const
{
    return bHasMaintenance_;
}

// -----------------------------------------------------------------------------
// Name: TypeComposante::HasMedical
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool TypeComposante::HasMedical() const
{
    return bHasMedical_;
}

// -----------------------------------------------------------------------------
// Name: TypeComposante::HasSupply
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool TypeComposante::HasSupply() const
{
    return bHasSupply_;
}
