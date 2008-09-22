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
#include "clients_gui/CheckBoxDisplayer.h"
#include "clients_gui/SpinBoxDisplayer.h"
#include "game_asn/SimulationSenders.h"
#include "clients_kernel/Object_ABC.h"

using namespace kernel;

typedef gui::ObjectPanel MyParent;

// -----------------------------------------------------------------------------
// Name: ObjectPanel constructor
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
ObjectPanel::ObjectPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, Publisher_ABC& publisher )
    : MyParent( parent, panel, controllers, factory )
    , publisher_( publisher )
{
    // $$$$ AGE 2006-08-23: tous ces trucs doivent etre identiques au labels utilisés
    // $$$$ AGE 2006-08-23: par le modèle correspondant et pire : traduits de la même maniere.
    // $$$$ AGE 2006-08-23: Faire des fichiers avec un tas de statics référencés par les 2 ?
    GetBuilder().AddGroup( tr( "Information" ) )
                .AddLabel( tr( "Identifier:" ) )
                .AddLabel( tr( "Name:" ) )
                .AddLabel( tr( "Type:" ) )
                .AddLabel( tr( "Location:" ) )
                .AddSpinBox( tr( "Construction:" ), 0, 100, 1 )
                .AddSpinBox( tr( "Mining:" ), 0, 100, 1 )
                .AddSpinBox( tr( "Bypass:" ), 0, 100, 1 )
                .AddLabel( tr( "Obstacle type:" ) )
                .AddCheckBox( tr( "Reserved obstacle activated:" ) );

    GetBuilder().Group( tr( "Information" ) )
                .AddLabel( tr( "Construction dotation:" ) )
                .AddLabel( tr( "Development dotation:" ) );

    GetBuilder().AddGroup( tr( "Crossing site" ) )
                .AddLabel( tr( "Width:" ) )
                .AddLabel( tr( "Depth:" ) )
                .AddLabel( tr( "Stream speed:" ) )
                .AddLabel( tr( "Build river banks:" ) );

    GetBuilder().AddGroup( tr( "Camp" ) )
                .AddLabel( tr( "TC2:" ) );

    GetBuilder().AddGroup( tr( "NBC cloud/zone" ) )
                .AddLabel( tr( "NBC agent:" ) );

    GetBuilder().AddGroup( tr( "ROTA" ) )
                .AddLabel( tr( "Danger level:" ) )
                .AddLabel( tr( "NBC agent(s):" ) );

    GetBuilder().AddGroup( tr( "Logistic route" ) )
                .AddLabel( tr( "Equipped:" ) )
                .AddLabel( tr( "Flow:" ) )
                .AddLabel( tr( "Width:" ) )
                .AddLabel( tr( "Length:" ) )
                .AddLabel( tr( "Maximum weight:" ) );

    GetBuilder().AddGroup( tr( "Mine parameters" ) )
                .AddLabel( tr( "Activity time:" ) )
                .AddLabel( tr( "Density:" ) );

    Displayer_ABC& infos = GetBuilder().Group( tr( "Information" ) );
    construction_  = dynamic_cast< gui::SpinBoxDisplayer* > ( & infos.Item( tr( "Construction:" ) ) );
    valorisation_  = dynamic_cast< gui::SpinBoxDisplayer* > ( & infos.Item( tr( "Mining:" ) ) );
    contournement_ = dynamic_cast< gui::SpinBoxDisplayer* > ( & infos.Item( tr( "Bypass:" ) ) );
//    prepared_      = dynamic_cast< CheckBoxDisplayer* >( & infos.Item( tr( "Prepared:" ) ) );

    QWidget* pSpacer = new QWidget( this );
    pSpacer->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );

    QHBox* pHBox  = new QHBox( this );
    QPushButton* pApplyButton_  = new QPushButton( tr( "Apply" ), pHBox );
    QPushButton* pCancelButton_ = new QPushButton( tr( "Cancel" )  , pHBox );

    connect( pApplyButton_,  SIGNAL( clicked() ), this, SLOT( OnApply() ) );
    connect( pCancelButton_, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel destructor
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
ObjectPanel::~ObjectPanel()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ObjectPanel::OnApply
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
void ObjectPanel::OnApply()
{
    const kernel::Object_ABC* object = GetSelected();
    if( object )
    {
        ASN1T_MagicActionUpdateObject asnAction;

        asnAction.m.construction_percentagePresent        = 1;
        asnAction.m.mining_percentagePresent              = 1;
        asnAction.m.bypass_construction_percentagePresent = 1;

        asnAction.oid                                     = object->GetId();
        asnAction.construction_percentage                 = construction_ ->GetValue();
        asnAction.mining_percentage                       = valorisation_ ->GetValue();
        asnAction.bypass_construction_percentage          = contournement_->GetValue();
//        asnAction.en_preparation                     = prepared_     ->IsChecked();

        simulation::ObjectMagicAction asnMsg;
        asnMsg().action.t                 = T_MsgObjectMagicAction_action_update_object;
        asnMsg().action.u.update_object   = &asnAction;

        asnMsg.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::OnCancel
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
void ObjectPanel::OnCancel()
{
    Update();
}
