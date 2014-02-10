// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChangeDiplomacyDialog.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Karma.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog constructor
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::ChangeDiplomacyDialog( QWidget* parent,
                                              Controllers& controllers,
                                              actions::ActionsModel& actionsModel,
                                              const Profile_ABC& profile )
    : gui::DiplomacyDialog_ABC( parent, controllers, profile )
    , actionsModel_( actionsModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog destructor
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::~ChangeDiplomacyDialog()
{
    // NOTHING
}

namespace
{
    sword::EnumDiplomacy ResolveDiplomacy( const Karma& karma )
    {
        if( karma == Karma::friend_ )
            return sword::friendly;
        if( karma == Karma::enemy_ )
            return sword::enemy;
        if( karma == Karma::neutral_ )
            return sword::neutral;
        return sword::unknown;
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::SetDiplomacy
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::SetDiplomacy( const Team_ABC& team1,
                                          const Team_ABC& team2,
                                          const Karma& diplomacy ) const
{
    if( team1.Get< Diplomacies_ABC >().GetDiplomacy( team2 ) == diplomacy )
        return;
    actionsModel_.PublishChangeDiplomacy( team1.GetId(), team2.GetId(), ResolveDiplomacy( diplomacy ) );
}
