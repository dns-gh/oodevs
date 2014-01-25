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
    GetBuilder().AddGroup( tools::translate( "Object", "Information" ) )
                .AddLabel( "Identifier", tools::translate( "Object", "Identifier:" ) )
                .AddLabel( "Name", tools::translate( "Object", "Name:" ) )
                .AddLabel( "type", tools::translate( "Object", "Type:" ) )
                .AddLabel( "Location", tools::translate( "Object", "Location:" ) );

    GetBuilder().Group( tools::translate( "Object", "Information" ) )
                .AddLabel( "Construction", tools::translate( "Object", "Construction:" ) )
                .AddLabel( "ConstructionResource", tools::translate( "Object", "Construction resource:" ) )
                .AddLabel( "Mining", tools::translate( "Object", "Mining:" ) )
                .AddLabel( "DevelopmentResource", tools::translate( "Object", "Development resource:" ) )
                .AddLabel( "Bypass", tools::translate( "Object", "Bypass:" ) )
                .AddLabel( "ObstacleActivated", tools::translate( "Object", "Obstacle activated:" ) )
                .AddLabel( "ActivationTime", tools::translate( "Object", "Activation time:" ) )
                .AddLabel( "ActivityTime", tools::translate( "Object", "Activity time:" ) )
                .AddLabel( "DelayTime", tools::translate( "Object", "Delay time:" ) );

    GetBuilder().AddGroup( tools::translate( "Object", "Crossing site" ) )
                .AddLabel( "Width", tools::translate( "Object", "Width:" ) )
                .AddLabel( "Depth", tools::translate( "Object", "Depth:" ) )
                .AddLabel( "Stream", tools::translate( "Object", "Stream speed:" ) )
                .AddLabel( "Build", tools::translate( "Object", "Build river banks:" ) );

    GetBuilder().AddGroup( tools::translate( "Object", "Camp" ) )
                .AddLabel( "Build", tools::translate( "Object", "Logistic superior:" ) )
                .AddLabel( "capacity", tools::translate( "Object", "Lodging capacity:" ) );

    GetBuilder().AddGroup( tools::translate( "NBC", "NBC" ) )
                .AddLabel( "Danger", tools::translate( "NBC", "Danger:" ) )
                .AddLabel( "NBCAgent", tools::translate( "NBC", "NBC agent(s):" ) );

    GetBuilder().AddGroup( tools::translate( "Object", "Logistic route" ) )
                .AddLabel( "Equipped", tools::translate( "Object", "Equipped:" ) )
                .AddLabel( "Flow", tools::translate( "Object", "Flow:" ) )
                .AddLabel( "Width", tools::translate( "Object", "Width:" ) )
                .AddLabel( "Length", tools::translate( "Object", "Length:" ) )
                .AddLabel( "Maximumheight", tools::translate( "Object", "Maximum weight:" ) );

    GetBuilder().AddGroup( tools::translate( "Object", "Mine parameters" ) )
                .AddLabel( "Density", tools::translate( "Object", "Density:" ) )
                .AddLabel( "Activity", tools::translate( "Object", "Activity time:" ) )
                .AddLabel( "Life", tools::translate( "Object", "Life time:" ) );

    GetBuilder().AddGroup( tools::translate( "Object", "Fire" ) )
                .AddLabel( "Fire", tools::translate( "Object", "Fire class:" ) )
                .AddLabel( "combustion", tools::translate( "Object", "Max combustion energy:" ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel destructor
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
ObjectPanel::~ObjectPanel()
{
    // NOTHING
}
