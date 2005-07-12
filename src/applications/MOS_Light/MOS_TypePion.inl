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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_TypePion.inl $
// $Author: Nld $
// $Modtime: 14/02/05 11:44 $
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
