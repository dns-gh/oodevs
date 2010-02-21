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
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/publisher_ABC.h"
#include "protocol/simulationsenders.h"

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
                .AddLabel( tr( "Location:" ) );

    GetBuilder().Group( tr( "Information" ) )
                .AddSpinBox( tr( "Construction:" ), 0, 100, 1 )
                .AddLabel( tr( "Construction dotation:" ) )
                .AddSpinBox( tr( "Mining:" ), 0, 100, 1 )
                .AddLabel( tr( "Development dotation:" ) )
                .AddSpinBox( tr( "Bypass:" ), 0, 100, 1 )                
                .AddLabel( tr( "Obstacle type:" ) )
                .AddCheckBox( tr( "Reserved obstacle activated:" ) );

    GetBuilder().AddGroup( tr( "Crossing site" ) )
                .AddLabel( tr( "Width:" ) )
                .AddLabel( tr( "Depth:" ) )
                .AddLabel( tr( "Stream speed:" ) )
                .AddLabel( tr( "Build river banks:" ) );

    GetBuilder().AddGroup( tr( "Camp" ) )
                .AddLabel( tr( "TC2:" ) );

    GetBuilder().AddGroup( tr( "NBC" ) )
                .AddLabel( tr( "NBC agent state:" ) )
                .AddLabel( tr( "Danger:" ) )
                .AddLabel( tr( "NBC agent(s):" ) );

    GetBuilder().AddGroup( tr( "Medical Treatment" ) )
                .AddLabel( tr( "Total number of beds:" ) )
                .AddLabel( tr( "Number of available beds:" ) )
                .AddLabel( tr( "Total number of doctors:" ) )
                .AddLabel( tr( "Number of available doctors:" ) );

    GetBuilder().AddGroup( tr( "Logistic route" ) )
                .AddLabel( tr( "Equipped:" ) )
                .AddLabel( tr( "Flow:" ) )
                .AddLabel( tr( "Width:" ) )
                .AddLabel( tr( "Length:" ) )
                .AddLabel( tr( "Maximum weight:" ) );

    GetBuilder().AddGroup( tr( "Mine parameters" ) )
                .AddLabel( tr( "Density:" ) )
                .AddLabel( tr( "Activity time:" ) );

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
        MsgsClientToSim::MsgMagicActionUpdateObject magicAction;
        magicAction.set_oid( object->GetId() );

        Displayer_ABC& infos = GetBuilder().Group( tr( "Information" ) );
        gui::CheckBoxDisplayer* pCheckBox = dynamic_cast< gui::CheckBoxDisplayer* > ( & infos.Item( tools::translate( "Object", "Reserved obstacle activated:" ) ) );
        if( pCheckBox && pCheckBox->IsChecked() )
        {
            magicAction.mutable_attributes()->mutable_obstacle()->set_type( Common::ObstacleType_DemolitionTargetType_reserved );
            magicAction.mutable_attributes()->mutable_obstacle()->set_activated( true );
        }

        magicAction.mutable_attributes()->mutable_construction()->set_percentage( construction_ ->GetValue() );
        magicAction.mutable_attributes()->mutable_mine()->set_percentage( valorisation_ ->GetValue() );
        magicAction.mutable_attributes()->mutable_bypass()->set_percentage( contournement_->GetValue() );

        simulation::ObjectMagicAction message;
        *message().mutable_action()->mutable_update_object() = magicAction;
        message.Send( publisher_ );
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
