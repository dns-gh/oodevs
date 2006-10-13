// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Dialogs.h"
#include "ChangeDiplomacyDialog.h"
#include "ChangeLogisticLinksDialog.h"
#include "LogisticSupplyChangeQuotasDialog.h"
#include "LogisticSupplyPushFlowDialog.h"
#include "LogisticSupplyRecompletionDialog.h"
#include "ChangeHumanFactorsDialog.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Dialogs constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Dialogs::Dialogs( QWidget* parent, Controllers& controllers, const Model& model, const StaticModel& staticModel, Publisher_ABC& publisher, const kernel::Profile_ABC& profile )
    : QObject( parent )
{
    new ChangeDiplomacyDialog( parent, controllers, publisher, profile );
    new ChangeLogisticLinksDialog( parent, controllers, publisher, profile  );
    new LogisticSupplyChangeQuotasDialog( parent, controllers, publisher, model, profile  );
    new LogisticSupplyPushFlowDialog( parent, controllers, publisher, model, profile  );
    new LogisticSupplyRecompletionDialog( parent, controllers, publisher, staticModel, profile );
    new ChangeHumanFactorsDialog( parent, controllers, publisher, profile );
}

// -----------------------------------------------------------------------------
// Name: Dialogs destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Dialogs::~Dialogs()
{
    // NOTHING
}
