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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_TypePion.inl $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: MOS_TypePion.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_TypePion::GetName
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const std::string& MOS_TypePion::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: MOS_TypePion::GetID
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
uint MOS_TypePion::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion::GetModel
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const MOS_AgentModel& MOS_TypePion::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion::IsLogistic
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool MOS_TypePion::IsLogistic() const
{
    return HasMaintenance() || HasMedical() || HasSupply();
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion::HasMaintenance
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool MOS_TypePion::HasMaintenance() const
{
    return bHasMaintenance_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion::HasMedical
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool MOS_TypePion::HasMedical() const
{
    return bHasMedical_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion::HasSupply
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
bool MOS_TypePion::HasSupply() const
{
    return bHasSupply_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion::GetStocks
// Created: HME 2005-11-03
// -----------------------------------------------------------------------------
inline
const MOS_TypePion::T_ResourceQty_Map& MOS_TypePion::GetStocks() const
{
    return stocks_;
}