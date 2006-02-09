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
// $Archive: /MVW_v10/Build/SDK/Light2/src/TypePion.inl $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: TypePion.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: TypePion::GetName
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const std::string& TypePion::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: TypePion::GetID
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
uint TypePion::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: TypePion::GetModel
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const AgentModel& TypePion::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: TypePion::IsLogistic
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool TypePion::IsLogistic() const
{
    return HasMaintenance() || HasMedical() || HasSupply();
}

// -----------------------------------------------------------------------------
// Name: TypePion::HasMaintenance
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool TypePion::HasMaintenance() const
{
    return bHasMaintenance_;
}

// -----------------------------------------------------------------------------
// Name: TypePion::HasMedical
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool TypePion::HasMedical() const
{
    return bHasMedical_;
}

// -----------------------------------------------------------------------------
// Name: TypePion::HasSupply
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool TypePion::HasSupply() const
{
    return bHasSupply_;
}

// -----------------------------------------------------------------------------
// Name: TypePion::GetStocks
// Created: HME 2005-11-03
// -----------------------------------------------------------------------------
inline
const TypePion::T_ResourceQty_Map& TypePion::GetStocks() const
{
    return stocks_;
}