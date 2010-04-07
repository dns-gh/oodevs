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
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Karma.h"
#include "protocol/simulationsenders.h"
#include "protocol/ServerPublisher_ABC.h"
// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog constructor
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::ChangeDiplomacyDialog( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile )
    : gui::DiplomacyDialog_ABC( parent, controllers, profile )
    , publisher_( publisher )
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
    Common::EnumDiplomacy ResolveDiplomacy( const kernel::Karma& karma )
    {
        if( karma == kernel::Karma::friend_ )
            return Common::friend_diplo;
        if( karma == kernel::Karma::enemy_ )
            return Common::enemy_diplo;
        if( karma == kernel::Karma::neutral_ )
            return Common::neutral_diplo;
        return Common::unknown_diplo;
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::SetDiplomacy
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::SetDiplomacy( const kernel::Team_ABC& team1, const kernel::Team_ABC& team2, const kernel::Karma& diplomacy ) const
{
    if( team1.Get< kernel::Diplomacies_ABC >().GetDiplomacy( team2 ) == diplomacy )
        return;
    simulation::ChangeDiplomacy message;
    message().set_oid_camp1( team1.GetId() );
    message().set_oid_camp2( team2.GetId() );
    message().set_diplomatie( ResolveDiplomacy( diplomacy ) );
    message.Send( publisher_ );
}
