// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Dialogs.h"
#include "ChangeDiplomacyDialog.h"
#include "ChangeLogisticLinksDialog.h"
#include "LogisticSupplyChangeQuotasDialog.h"

// -----------------------------------------------------------------------------
// Name: Dialogs constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Dialogs::Dialogs( QWidget* parent, Controllers& controllers, const Model& model )
{
    new ChangeDiplomacyDialog( parent, controllers );
    new ChangeLogisticLinksDialog( parent, controllers );
    new LogisticSupplyChangeQuotasDialog( parent, controllers, model );
}

// -----------------------------------------------------------------------------
// Name: Dialogs destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Dialogs::~Dialogs()
{
    // NOTHING
}
