// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamAgent.inl $
// $Author: Ape $
// $Modtime: 24/03/04 11:45 $
// $Revision: 1 $
// $Workfile: MOS_ParamAgent.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_ParamAgent::Clear
// Created: HME 2005-08-02
// -----------------------------------------------------------------------------
inline
void MOS_ParamAgent::Clear()
{
    pAgentLabel_->setText( tr( "---" ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_ParamAgent::GetAgent
// Created: HME 2005-08-04
// -----------------------------------------------------------------------------
inline
MOS_Agent* MOS_ParamAgent::GetAgent()
{
    return pAgent_;
}