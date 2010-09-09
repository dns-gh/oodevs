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
#include "BriefingDialog.h"
#include "ChangeDiplomacyDialog.h"
#include "ChangeHumanFactorsDialog.h"
#include "ChangeLogisticLinksDialog.h"
#include "LogisticSupplyChangeQuotasDialog.h"
#include "LogisticSupplyPushFlowDialog.h"
#include "LogisticSupplyRecompletionDialog.h"
#include "gaming/AgentsModel.h"
#include "gaming/Model.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Dialogs constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Dialogs::Dialogs( QWidget* parent, Controllers& controllers, const Model& model, const StaticModel& staticModel, Publisher_ABC& publisher, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile, CommandHandler& handler, const tools::ExerciseConfig& config )
    : QObject( parent )
{
    new ChangeDiplomacyDialog( parent, controllers, actionsModel, staticModel, simulation, profile );
    new ChangeLogisticLinksDialog( parent, controllers, actionsModel, staticModel, simulation, profile  );
    new LogisticSupplyChangeQuotasDialog( parent, controllers, actionsModel, staticModel, simulation, model, profile  );
    new LogisticSupplyPushFlowDialog( parent, controllers, actionsModel, staticModel, simulation, model.agents_, profile  );
    new LogisticSupplyRecompletionDialog( parent, controllers, staticModel, actionsModel, simulation, profile );
    new ChangeHumanFactorsDialog( parent, controllers, staticModel, actionsModel, simulation, profile );
    new BriefingDialog( parent, controllers, publisher, handler, config );
}

// -----------------------------------------------------------------------------
// Name: Dialogs destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Dialogs::~Dialogs()
{
    // NOTHING
}
