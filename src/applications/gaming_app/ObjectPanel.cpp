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
#include "actions/ActionTiming.h"
#include "actions/ObjectMagicAction.h"
#include "actions/ParameterList.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/GroupDisplayer.h"
#include "clients_gui/LabelDisplayer.h"
#include "clients_gui/CheckBoxDisplayer.h"
#include "clients_gui/SpinBoxDisplayer.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/StaticModel.h"
#include "protocol/simulationsenders.h"

using namespace actions;
using namespace kernel;
using namespace parameters;

typedef gui::ObjectPanel MyParent;

// -----------------------------------------------------------------------------
// Name: ObjectPanel constructor
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
ObjectPanel::ObjectPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation )
    : MyParent( parent, panel, controllers, factory )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
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

    GetBuilder().AddGroup( tr( "Fire" ) )
                .AddLabel( tr( "Fire class:" ) )
                .AddLabel( tr( "Fire temperature:" ) );

    GetBuilder().AddGroup( tr( "Decisionnal" ) )
                .AddLabel( tr( "Sealing level:" ) );

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
        // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "update_object" );
        ObjectMagicAction* action = new ObjectMagicAction( object, actionType, controllers_.controller_, true );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();

        ParameterList* attributesList = new ParameterList( it.NextElement() );
        action->AddParameter( *attributesList );

        // add attributes
        Displayer_ABC& infos = GetBuilder().Group( tr( "Information" ) );
        gui::CheckBoxDisplayer* pCheckBox = dynamic_cast< gui::CheckBoxDisplayer* > ( & infos.Item( tools::translate( "ObjectPanel", "Reserved obstacle activated:" ) ) );
        if( pCheckBox && pCheckBox->IsChecked() )
        {
            ParameterList& obstacleList = attributesList->AddList( "Obstacle" );
            obstacleList.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_obstacle );
            obstacleList.AddIdentifier( "TargetType", Common::ObstacleType_DemolitionTargetType_reserved );
            obstacleList.AddBool( "Activation", true );
        }

        ParameterList& constructionList = attributesList->AddList( "Construction" );
        constructionList.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_construction );
        constructionList.AddIdentifier( "Type", 0 );
        constructionList.AddQuantity( "Number", 0 );
        constructionList.AddNumeric( "Density", 0 );
        constructionList.AddQuantity( "Percentage", construction_->GetValue() );

        ParameterList& mineList = attributesList->AddList( "Mine" );
        mineList.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_mine );
        mineList.AddIdentifier( "Type", 0 );
        mineList.AddQuantity( "Number", 0 );
        mineList.AddNumeric( "Density", 0 );
        mineList.AddQuantity( "Percentage", valorisation_->GetValue() );

        ParameterList& bypassList = attributesList->AddList( "Bypass" );
        bypassList.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_bypass );
        bypassList.AddQuantity( "Percentage", contournement_->GetValue() );

        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->RegisterAndPublish( actionsModel_ );
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
