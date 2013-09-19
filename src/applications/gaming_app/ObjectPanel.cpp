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
                .AddLabel( "Identifier", tools::findTranslation( "Object", "Identifier:" ) )
                .AddLabel( "Name", tools::findTranslation( "Object", "Name:" ) )
                .AddLabel( "type", tools::findTranslation( "Object", "Type:" ) )
                .AddLabel( "Location", tools::findTranslation( "Object", "Location:" ) );

    GetBuilder().Group( tools::findTranslation( "Object", "Information" ) )
                .AddLabel( "Construction", tools::findTranslation( "Object", "Construction:" ) )
                .AddLabel( "ConstructionResource", tools::findTranslation( "Object", "Construction resource:" ) )
                .AddLabel( "Mining", tools::findTranslation( "Object", "Mining:" ) )
                .AddLabel( "DevelopmentResource", tools::findTranslation( "Object", "Development resource:" ) )
                .AddLabel( "Bypass", tools::findTranslation( "Object", "Bypass:" ) )
                .AddLabel( "ObstacleActivated", tools::findTranslation( "Object", "Obstacle activated:" ) )
                .AddLabel( "ActivationTime", tools::findTranslation( "Object", "Activation time:" ) )
                .AddLabel( "ActivityTime", tools::findTranslation( "Object", "Activity time:" ) )
                .AddLabel( "DelayTime", tools::findTranslation( "Object", "Delay time:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Crossing site" ) )
                .AddLabel( "Width", tools::findTranslation( "Object", "Width:" ) )
                .AddLabel( "Depth", tools::findTranslation( "Object", "Depth:" ) )
                .AddLabel( "Stream", tools::findTranslation( "Object", "Stream speed:" ) )
                .AddLabel( "Build", tools::findTranslation( "Object", "Build river banks:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Camp" ) )
                .AddLabel( "Build", tools::findTranslation( "Object", "Logistic superior:" ) )
                .AddLabel( "capacity", tools::findTranslation( "Object", "Lodging capacity:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "NBC", "NBC" ) )
                .AddLabel( "Danger", tools::findTranslation( "NBC", "Danger:" ) )
                .AddLabel( "NBCAgent", tools::findTranslation( "NBC", "NBC agent(s):" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Logistic route" ) )
                .AddLabel( "Equipped", tools::findTranslation( "Object", "Equipped:" ) )
                .AddLabel( "Flow", tools::findTranslation( "Object", "Flow:" ) )
                .AddLabel( "Width", tools::findTranslation( "Object", "Width:" ) )
                .AddLabel( "Length", tools::findTranslation( "Object", "Length:" ) )
                .AddLabel( "Maximumheight", tools::findTranslation( "Object", "Maximum weight:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Mine parameters" ) )
                .AddLabel( "Density", tools::findTranslation( "Object", "Density:" ) )
                .AddLabel( "Activity", tools::findTranslation( "Object", "Activity time:" ) )
                .AddLabel( "Life", tools::findTranslation( "Object", "Life time:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Fire" ) )
                .AddLabel( "Fire", tools::findTranslation( "Object", "Fire class:" ) )
                .AddLabel( "combustion", tools::findTranslation( "Object", "Max combustion energy:" ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel destructor
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
ObjectPanel::~ObjectPanel()
{
    // NOTHING
}
