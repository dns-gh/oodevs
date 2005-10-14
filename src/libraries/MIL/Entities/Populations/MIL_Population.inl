// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDefaultAttitude
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
inline
const MIL_PopulationAttitude& MIL_Population::GetDefaultAttitude() const
{
    assert( pDefaultAttitude_ );
    return *pDefaultAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetID
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
uint MIL_Population::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetType
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
const MIL_PopulationType& MIL_Population::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetName
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
const std::string& MIL_Population::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDecision
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
DEC_PopulationDecision& MIL_Population::GetDecision() const
{
    assert( pDecision_ );
    return *pDecision_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetOrderManager
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
inline
const MIL_PopulationOrderManager& MIL_Population::GetOrderManager() const
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetOrderManager
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
inline
MIL_PopulationOrderManager& MIL_Population::GetOrderManager()
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetArmy
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
const MIL_Army& MIL_Population::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgPopulationOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void MIL_Population::OnReceiveMsgPopulationOrder( ASN1T_MsgPopulationOrder& msg, MIL_MOSContextID nCtx )
{
    orderManager_.OnReceiveMsgPopulationOrder( msg, nCtx );

}