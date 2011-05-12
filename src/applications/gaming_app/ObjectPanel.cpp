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
#include "protocol/SimulationSenders.h"

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
    GetBuilder().AddGroup( tools::findTranslation( "Object", "Information" ) )
                .AddLabel( tools::findTranslation( "Object", "Identifier:" ) )
                .AddLabel( tools::findTranslation( "Object", "Name:" ) )
                .AddLabel( tools::findTranslation( "Object", "Type:" ) )
                .AddLabel( tools::findTranslation( "Object", "Location:" ) );

    GetBuilder().Group( tools::findTranslation( "Object", "Information" ) )
                .AddSpinBox( tools::findTranslation( "Object", "Construction:" ), 0, 100, 1 )
                .AddLabel( tools::findTranslation( "Object", "Construction dotation:" ) )
                .AddSpinBox( tools::findTranslation( "Object", "Mining:" ), 0, 100, 1 )
                .AddLabel( tools::findTranslation( "Object", "Development dotation:" ) )
                .AddSpinBox( tools::findTranslation( "Object", "Bypass:" ), 0, 100, 1 )
                .AddLabel( tools::findTranslation( "Object", "Obstacle type:" ) )
                .AddCheckBox( tools::findTranslation( "Object", "Reserved obstacle activated:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Crossing site" ) )
                .AddLabel( tools::findTranslation( "Object", "Width:" ) )
                .AddLabel( tools::findTranslation( "Object", "Depth:" ) )
                .AddLabel( tools::findTranslation( "Object", "Stream speed:" ) )
                .AddLabel( tools::findTranslation( "Object", "Build river banks:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Camp" ) )
                .AddLabel( tools::findTranslation( "Object", "TC2:" ) )
                .AddLabel( tools::findTranslation( "Object", "Lodging capacity:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "NBC", "NBC" ) )
                .AddLabel( tools::findTranslation( "NBC", "Danger:" ) )
                .AddLabel( tools::findTranslation( "NBC", "NBC agent(s):" ) );

    GetBuilder().AddGroup( tools::findTranslation( "MedicalTreatment", "Medical Treatment" ) )
                .AddLabel( tools::findTranslation( "MedicalTreatment", "Total number of beds:" ) )
                .AddLabel( tools::findTranslation( "MedicalTreatment", "Number of available beds:" ) )
                .AddLabel( tools::findTranslation( "MedicalTreatment", "Total number of doctors:" ) )
                .AddLabel( tools::findTranslation( "MedicalTreatment", "Number of available doctors:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Logistic route" ) )
                .AddLabel( tools::findTranslation( "Object", "Equipped:" ) )
                .AddLabel( tools::findTranslation( "Object", "Flow:" ) )
                .AddLabel( tools::findTranslation( "Object", "Width:" ) )
                .AddLabel( tools::findTranslation( "Object", "Length:" ) )
                .AddLabel( tools::findTranslation( "Object", "Maximum weight:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Mine parameters" ) )
                .AddLabel( tools::findTranslation( "Object", "Density:" ) )
                .AddLabel( tools::findTranslation( "Object", "Activity time:" ) );

    GetBuilder().AddGroup( tools::findTranslation( "Object", "Fire" ) )
                .AddLabel( tools::findTranslation( "Object", "Fire class:" ) )
                .AddLabel( tools::findTranslation( "Object", "Fire temperature:" ) );

    Displayer_ABC& infos = GetBuilder().Group( tools::findTranslation( "Object", "Information" ) );
    construction_  = dynamic_cast< gui::SpinBoxDisplayer* > ( & infos.Item( tools::findTranslation( "Object", "Construction:" ) ) );
    valorisation_  = dynamic_cast< gui::SpinBoxDisplayer* > ( & infos.Item( tools::findTranslation( "Object", "Mining:" ) ) );
    contournement_ = dynamic_cast< gui::SpinBoxDisplayer* > ( & infos.Item( tools::findTranslation( "Object", "Bypass:" ) ) );

    QWidget* pSpacer = new QWidget( this );
    pSpacer->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );

    QHBox* pHBox  = new QHBox( this );
    QPushButton* pApplyButton_  = new QPushButton( tools::translate( "CreationPanel", "Apply" ), pHBox );
    QPushButton* pCancelButton_ = new QPushButton( tools::translate( "CreationPanel", "Cancel" ), pHBox );

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
        action->Rename( tools::translate( "gaming_app::Action", "Object Update" ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();

        ParameterList* attributesList = new ParameterList( it.NextElement() );
        action->AddParameter( *attributesList );

        // add attributes
        Displayer_ABC& infos = GetBuilder().Group( tools::findTranslation( "Object", "Information" ) );
        gui::CheckBoxDisplayer* pCheckBox = dynamic_cast< gui::CheckBoxDisplayer* > ( & infos.Item( tools::findTranslation( "Object", "Reserved obstacle activated:" ) ) );
        if( pCheckBox && pCheckBox->IsChecked() )
        {
            ParameterList& obstacleList = attributesList->AddList( "Obstacle" );
            obstacleList.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_obstacle );
            obstacleList.AddIdentifier( "TargetType", sword::ObstacleType_DemolitionTargetType_reserved );
            obstacleList.AddBool( "Activation", true );
        }

        ParameterList& constructionList = attributesList->AddList( "Construction" );
        constructionList.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_construction );
        constructionList.AddIdentifier( "Type", 0 );
        constructionList.AddQuantity( "Number", 0 );
        constructionList.AddNumeric( "Density", 0 );
        constructionList.AddQuantity( "Percentage", construction_->GetValue() );

        ParameterList& mineList = attributesList->AddList( "Mine" );
        mineList.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_mine );
        mineList.AddIdentifier( "Type", 0 );
        mineList.AddQuantity( "Number", 0 );
        mineList.AddNumeric( "Density", 0 );
        mineList.AddQuantity( "Percentage", valorisation_->GetValue() );

        ParameterList& bypassList = attributesList->AddList( "Bypass" );
        bypassList.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_bypass );
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
