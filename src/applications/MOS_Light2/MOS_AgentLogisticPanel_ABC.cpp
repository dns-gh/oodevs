// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-01 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentLogisticPanel_ABC.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 18:27 $
// $Revision: 3 $
// $Workfile: MOS_AgentLogisticPanel_ABC.cpp $
//
// *****************************************************************************

#include "MOS_Light2_Pch.h"
#include "MOS_AgentLogisticPanel_ABC.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"

// -----------------------------------------------------------------------------
// Name: MOS_AgentLogisticPanel_ABC constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_AgentLogisticPanel_ABC::MOS_AgentLogisticPanel_ABC( QWidget* pParent )
    : MOS_InfoPanel_ABC( pParent )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentLogisticPanel_ABC destructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_AgentLogisticPanel_ABC::~MOS_AgentLogisticPanel_ABC()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentLogisticPanel_ABC::MOS_App::GetApp().GetEquipmentName
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
std::string MOS_AgentLogisticPanel_ABC::EquipmentResolver::GetName( unsigned int n ) const
{
    return MOS_App::GetApp().GetEquipmentName( n );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentLogisticPanel_ABC::MOS_App::GetApp().GetEquipmentName
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
std::string MOS_AgentLogisticPanel_ABC::AutomateResolver::GetName( unsigned int n ) const
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( n );
    assert( pAgent );
    return pAgent->GetName();
}

 