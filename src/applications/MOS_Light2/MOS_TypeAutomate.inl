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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_TypeAutomate.inl $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: MOS_TypeAutomate.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_TypeAutomate::GetName
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const std::string& MOS_TypeAutomate::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: MOS_TypeAutomate::GetID
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
uint MOS_TypeAutomate::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypeAutomate::GetModel
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const MOS_AgentModel& MOS_TypeAutomate::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypeAutomate::GetTypePC
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const MOS_TypePion& MOS_TypeAutomate::GetTypePC() const
{
    assert( pTypePC_ );
    return *pTypePC_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypeAutomate::GetType
// Created: HME 2005-08-08
// -----------------------------------------------------------------------------
inline
const std::string MOS_TypeAutomate::GetType() const
{
    return strType_;
}
