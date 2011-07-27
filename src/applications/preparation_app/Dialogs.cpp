// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Dialogs.h"
#include "ChangeDiplomacyDialog.h"
#include "InitialStateDialog.h"
#include "AgentAffinitiesDialog.h"
#include "PeopleAffinitiesDialog.h"
#include "ColorEditor.h"
#include "SymbolEditor.h"
#include "clients_kernel/ObjectTypes.h"
#include "preparation/StaticModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Dialogs constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Dialogs::Dialogs( QWidget* parent, Controllers& controllers, const StaticModel& model, const kernel::Profile_ABC& profile,
                  gui::ColorStrategy_ABC& colorStrategy, gui::ColorEditor_ABC& colorEditor, const gui::EntitySymbols& symbols,
                  const tools::ExerciseConfig& config )
    : QObject( parent )
{
    new ChangeDiplomacyDialog( parent, controllers, profile );
    new InitialStateDialog( parent, controllers, model );
    new AgentAffinitiesDialog( parent, controllers );
    new PeopleAffinitiesDialog( parent, controllers );
    new ColorEditor( parent, controllers, colorStrategy, colorEditor );
    new SymbolEditor( parent, controllers, symbols, config );
}

// -----------------------------------------------------------------------------
// Name: Dialogs destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Dialogs::~Dialogs()
{
    // NOTHING
}
