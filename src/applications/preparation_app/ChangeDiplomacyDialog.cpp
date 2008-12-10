// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ChangeDiplomacyDialog.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/Diplomacies.h"

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog constructor
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::ChangeDiplomacyDialog( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile )
    : gui::DiplomacyDialog_ABC( parent, controllers, profile )
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

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::SetDiplomacy
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::SetDiplomacy( const kernel::Team_ABC& team1, const kernel::Team_ABC& team2, const kernel::Karma& diplomacy ) const
{
    if( Diplomacies* diplomacies = dynamic_cast< Diplomacies* >( const_cast< kernel::Team_ABC& >( team1 ).Retrieve< kernel::Diplomacies_ABC >() ) )
        diplomacies->SetDiplomacy( team2, diplomacy );
}
