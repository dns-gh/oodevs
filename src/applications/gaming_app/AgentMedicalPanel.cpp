// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentMedicalPanel.h"
#include "LogisticStatusWidgets.h"
#include "MedicalConsignsWidget.h"
#include "MedicalReliefAmbulancesListView.h"
#include "MedicalCollectAmbulancesListView.h"
#include "MedicalDoctorsListView.h"

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMedicalPanel::AgentMedicalPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : EntityPanel( parent, panel, controllers, tr( "Medical system" ) )
{
    new MedicalConsignsWidget( this, controllers, factory );
    new MedicalStatusWidget( this, controllers, factory );
    new MedicalReliefAmbulancesListView( this, controllers, factory );
    new MedicalCollectAmbulancesListView( this, controllers, factory );
    new MedicalDoctorsListView( this, controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMedicalPanel::~AgentMedicalPanel()
{
    // NOTHING
}
