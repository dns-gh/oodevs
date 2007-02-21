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

// -----------------------------------------------------------------------------
// Name: MedicalConsignsWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MedicalConsignsWidget::MedicalConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticConsignsWidget_ABC< MedicalConsignsWidget, LogMedicalConsign, MedicalStates >( parent, controllers, factory )
{
    AddConsignColumn( tr( "Injury:" ) );
    AddConsignColumn( tr( "Mentally injured:" ) );
    AddConsignColumn( tr( "NBC contaminated:" ) );
    AddConsignColumn( tr( "State:" ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignsWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MedicalConsignsWidget::~MedicalConsignsWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignsWidget::DisplayRequested
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void MedicalConsignsWidget::DisplayRequested( const LogisticConsigns& consigns, gui::ListDisplayer< MedicalConsignsWidget >* list )
{
    list->DeleteTail( 
        list->DisplayList( consigns.requestedMedical_.begin(), consigns.requestedMedical_.end() )
        );
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignsWidget::DisplayHandled
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void MedicalConsignsWidget::DisplayHandled( const LogisticConsigns& consigns, gui::ListDisplayer< MedicalConsignsWidget >* list )
{
    list->DeleteTail( 
        list->DisplayList( consigns.handledMedical_.begin(), consigns.handledMedical_.end() )
        );
}
