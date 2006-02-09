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
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogSupplyConsign.inl $
// $Author: Age $
// $Modtime: 1/04/05 11:07 $
// $Revision: 1 $
// $Workfile: LogSupplyConsign.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetID
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
uint LogSupplyConsign::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetPion
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
Agent& LogSupplyConsign::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetAutomateLogHandling
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
Agent* LogSupplyConsign::GetAutomateLogHandling() const
{
    return pAutomateLogHandling_;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetPionLogConvoying
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
inline
Agent* LogSupplyConsign::GetPionLogConvoying() const
{
    return pPionLogConvoying_;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
LogSupplyConsign::E_State LogSupplyConsign::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetDotations
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
inline
const LogSupplyConsign::T_DotationMap& LogSupplyConsign::GetDotations() const
{
    return dotations_;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetAutomateLogProvidingConvoyResources
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
inline
Agent* LogSupplyConsign::GetAutomateLogProvidingConvoyResources() const
{
    return pAutomateLogProvidingConvoyResources_;
}
