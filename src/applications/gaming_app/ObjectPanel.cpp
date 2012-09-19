// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectPanel.h"
#include "moc_ObjectPanel.cpp"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/GroupDisplayer.h"
#include "clients_gui/LabelDisplayer.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: ObjectPanel constructor
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
ObjectPanel::ObjectPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : gui::ObjectPanel( parent, panel, controllers, factory )
{
    // $$$$ AGE 2006-08-23: tous ces trucs doivent etre identiques au labels utilisés
    // $$$$ AGE 2006-08-23: par le modèle correspondant et pire : traduits de la même maniere.
    // $$$$ AGE 2006-08-23: Faire des fichiers avec un tas de statics référencés par les 2 ?
    GetBuilder().AddGroup( tools::findTranslation( "Object", "Information" ) )
                .AddLabel( tools::findTranslation( "Object", "Identifier:" ) )
                .AddLabel( tools::findTranslation( "Object", "Name:" ) )
                .AddLabel( tools::findTranslation( "Object", "Type:" ) )
                .AddLabel( tools::findTranslation( "Object", "Location:" ) );

    GetBuilder().Group( tools::findTranslation( "Object", "Information" ) )
                .AddLabel( tools::findTranslation( "Object", "Construction:" ) )
                .AddLabel( tools::findTranslation( "Object", "Construction resource:" ) )
                .AddLabel( tools::findTranslation( "Object", "Mining:" ) )
                .AddLabel( tools::findTranslation( "Object", "Development resource:" ) )
                .AddLabel( tools::findTranslation( "Object", "Bypass:" ) )
                .AddLabel( tools::findTranslation( "Object", "Obstacle type:" ) )
                .AddLabel( tools::findTranslation( "Object", "Obstacle activated:" ) )
                .AddLabel( tools::findTranslation( "Object", "Activation time:" ) )
                .AddLabel( tools::findTranslation( "Object", "Activity time:" ) )
                .AddLabel( tools::findTranslation( "Object", "Delay time:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Crossing site" ) )
                .AddLabel( tools::findTranslation( "Object", "Width:" ) )
                .AddLabel( tools::findTranslation( "Object", "Depth:" ) )
                .AddLabel( tools::findTranslation( "Object", "Stream speed:" ) )
                .AddLabel( tools::findTranslation( "Object", "Build river banks:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Camp" ) )
                .AddLabel( tools::findTranslation( "Object", "Logistic superior:" ) )
                .AddLabel( tools::findTranslation( "Object", "Lodging capacity:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "NBC", "NBC" ) )
                .AddLabel( tools::findTranslation( "NBC", "Danger:" ) )
                .AddLabel( tools::findTranslation( "NBC", "NBC agent(s):" ) );

    GetBuilder().AddGroup( tools::findTranslation( "MedicalTreatment", "Medical Treatment" ) )
                .AddLabel( tools::findTranslation( "MedicalTreatment", "Total number of beds:" ) )
                .AddLabel( tools::findTranslation( "MedicalTreatment", "Number of available beds:" ) )
                .AddLabel( tools::findTranslation( "MedicalTreatment", "Total number of doctors:" ) )
                .AddLabel( tools::findTranslation( "MedicalTreatment", "Number of available doctors:" ) )
                .AddLabel( tools::findTranslation( "MedicalTreatment", "Hospital ID:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "MedicalTreatment", "Medical Treatment services (Available(Baseline)):" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Logistic route" ) )
                .AddLabel( tools::findTranslation( "Object", "Equipped:" ) )
                .AddLabel( tools::findTranslation( "Object", "Flow:" ) )
                .AddLabel( tools::findTranslation( "Object", "Width:" ) )
                .AddLabel( tools::findTranslation( "Object", "Length:" ) )
                .AddLabel( tools::findTranslation( "Object", "Maximum weight:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Mine parameters" ) )
                .AddLabel( tools::findTranslation( "Object", "Density:" ) )
                .AddLabel( tools::findTranslation( "Object", "Activity time:" ) )
                .AddLabel( tools::findTranslation( "Object", "Life time:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Fire" ) )
                .AddLabel( tools::findTranslation( "Object", "Fire class:" ) )
                .AddLabel( tools::findTranslation( "Object", "Max combustion energy:" ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel destructor
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
ObjectPanel::~ObjectPanel()
{
    // NOTHING
}
