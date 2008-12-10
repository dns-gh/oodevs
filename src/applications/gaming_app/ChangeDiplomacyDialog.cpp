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
#include "game_asn/SimulationSenders.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Karma.h"

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
    ASN1T_EnumDiplomacy ResolveDiplomacy( const kernel::Karma& karma )
    {
        if( karma == kernel::Karma::friend_ )
            return EnumDiplomacy::ami;
        if( karma == kernel::Karma::enemy_ )
            return EnumDiplomacy::ennemi;
        if( karma == kernel::Karma::neutral_ )
            return EnumDiplomacy::neutre;
        return EnumDiplomacy::inconnu;
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
    message().oid_camp1  = team1.GetId();
    message().oid_camp2  = team2.GetId();
    message().diplomatie = ResolveDiplomacy( diplomacy );
    message.Send( publisher_ );
}
