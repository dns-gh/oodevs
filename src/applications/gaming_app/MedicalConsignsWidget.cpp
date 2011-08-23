// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MedicalConsignsWidget.h"
#include "gaming/LogMedicalConsign.h"
#include "gaming/MedicalStates.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: MedicalConsignsWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MedicalConsignsWidget::MedicalConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticConsignsWidget_ABC< MedicalConsignsWidget, LogMedicalConsign, LogMedicalConsigns >( parent, controllers, factory )
{
    AddConsignColumn( tools::translate( "LogisticConsignsWidget_ABC", "Consumer:" ) );
    AddConsignColumn( tools::translate( "LogisticConsignsWidget_ABC", "Handler:" ) );
    AddConsignColumn( tools::translate( "MedicalConsignsWidget", "Injury:" ) );
    AddConsignColumn( tools::translate( "MedicalConsignsWidget", "Mentally injured:" ) );
    AddConsignColumn( tools::translate( "MedicalConsignsWidget", "NBC contaminated:" ) );
    AddConsignColumn( tools::translate( "MedicalConsignsWidget", "State:" ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignsWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MedicalConsignsWidget::~MedicalConsignsWidget()
{
    // NOTHING
}
