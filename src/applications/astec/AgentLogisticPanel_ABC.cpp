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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentLogisticPanel_ABC.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 18:27 $
// $Revision: 3 $
// $Workfile: AgentLogisticPanel_ABC.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentLogisticPanel_ABC.h"
#include "App.h"
#include "AgentManager.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: AgentLogisticPanel_ABC constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentLogisticPanel_ABC::AgentLogisticPanel_ABC( QWidget* pParent )
    : InfoPanel_ABC( pParent )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentLogisticPanel_ABC destructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentLogisticPanel_ABC::~AgentLogisticPanel_ABC()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentLogisticPanel_ABC::App::GetApp().GetEquipmentName
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
std::string AgentLogisticPanel_ABC::EquipmentResolver::GetName( unsigned int n ) const
{
    return App::GetApp().GetEquipmentName( n );
}

// -----------------------------------------------------------------------------
// Name: AgentLogisticPanel_ABC::App::GetApp().GetEquipmentName
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
std::string AgentLogisticPanel_ABC::AutomateResolver::GetName( unsigned int n ) const
{
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( n );
    assert( pAgent );
    return pAgent->GetName();
}

 