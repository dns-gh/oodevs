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
// $Archive: /MVW_v10/Build/SDK/Light2/src/TypeAutomate.inl $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: TypeAutomate.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: TypeAutomate::GetName
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const std::string& TypeAutomate::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: TypeAutomate::GetID
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
uint TypeAutomate::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: TypeAutomate::GetModel
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const AgentModel& TypeAutomate::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: TypeAutomate::GetTypePC
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
inline
const TypePion& TypeAutomate::GetTypePC() const
{
    assert( pTypePC_ );
    return *pTypePC_;
}

// -----------------------------------------------------------------------------
// Name: TypeAutomate::GetType
// Created: HME 2005-08-08
// -----------------------------------------------------------------------------
inline
const std::string TypeAutomate::GetType() const
{
    return strType_;
}
