// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Dialogs.h"
#include "ChangeDiplomacyDialog.h"
#include "ChangeLogisticLinksDialog.h"
#include "LogisticSupplyChangeQuotasDialog.h"
#include "LogisticSupplyPushFlowDialog.h"
#include "LogisticSupplyRecompletionDialog.h"
#include "ChangeHumanFactorsDialog.h"

// -----------------------------------------------------------------------------
// Name: Dialogs constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Dialogs::Dialogs( QWidget* parent, Controllers& controllers, const Model& model, const StaticModel& staticModel, Publisher_ABC& publisher )
{
    new ChangeDiplomacyDialog( parent, controllers, publisher );
    new ChangeLogisticLinksDialog( parent, controllers, publisher );
    new LogisticSupplyChangeQuotasDialog( parent, controllers, publisher, model );
    new LogisticSupplyPushFlowDialog( parent, controllers, publisher, model );
    new LogisticSupplyRecompletionDialog( parent, controllers, publisher, staticModel );
    new ChangeHumanFactorsDialog( parent, controllers, publisher );
}

// -----------------------------------------------------------------------------
// Name: Dialogs destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Dialogs::~Dialogs()
{
    // NOTHING
}
