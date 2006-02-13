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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgent.inl $
// $Author: Ape $
// $Modtime: 24/03/04 11:45 $
// $Revision: 1 $
// $Workfile: ParamAgent.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ParamAgent::Clear
// Created: HME 2005-08-02
// -----------------------------------------------------------------------------
inline
void ParamAgent::Clear()
{
    pAgentLabel_->setText( tr( "---" ) );
}

// -----------------------------------------------------------------------------
// Name: ParamAgent::GetAgent
// Created: HME 2005-08-04
// -----------------------------------------------------------------------------
inline
Agent* ParamAgent::GetAgent()
{
    return pAgent_;
}