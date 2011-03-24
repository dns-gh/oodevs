// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectMagicOrdersInterface.h"
#include "moc_ObjectMagicOrdersInterface.cpp"
#include "actions/ActionsModel.h"
#include "actions/ParameterList.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/Object.h"
#include "protocol/SimulationSenders.h"

using namespace actions;
using namespace kernel;
using namespace parameters;

namespace
{
    // $$$$ _RC_ JSR 2011-03-01: Commun avec PopulationMagicOrdersInterface -> � mettre dans un fichier commun
    class InitializedLineEdit : public QLineEdit
    {
    public:
        InitializedLineEdit( QWidget* parent, const QString& initialValue )
            : QLineEdit( initialValue, parent ), initialValue_( initialValue )
        {
            setValidator( new QIntValidator( 0, 100, this ) );
        }

        virtual void keyPressEvent( QKeyEvent* e )
        {
            if( text() == initialValue_ )
            {
                clear();
                setAlignment( Qt::AlignRight );
            }
            QLineEdit::keyPressEvent( e );
        }

        virtual void showEvent( QShowEvent* e )
        {
            if( text() == initialValue_ )
            {
                selectAll();
                setFocus();
            }
            QLineEdit::showEvent( e );
        }

    private:
        QString initialValue_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ObjectMagicOrdersInterface::ObjectMagicOrdersInterface( QWidget* parent, Controllers& controllers, ActionsModel& actionsModel, const Profile_ABC& profile )
    : QObject( parent )
    , controllers_   ( controllers )
    , actionsModel_  ( actionsModel )
    , selectedEntity_( controllers )
    , profile_       ( profile )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ObjectMagicOrdersInterface::~ObjectMagicOrdersInterface()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) )
        return;
    selectedEntity_ = &entity;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    if( entity.GetType().IsUrban() )
    {
        AddValuedMagic( magicMenu, menu, tr( "Change Urban state" ), SLOT( ChangeStructuralState() ) );
        AddMagic( tr( "Alert" ), SLOT( Alert() ), magicMenu );
        AddMagic( tr( "Stop alert" ), SLOT( StopAlert() ), magicMenu );
        AddMagic( tr( "Confine" ), SLOT( Confine() ), magicMenu );
        AddMagic( tr( "Stop confine" ), SLOT( StopConfine() ), magicMenu );
        AddMagic( tr( "Evacuate" ), SLOT( Evacuate() ), magicMenu );
        AddMagic( tr( "Stop evacuate" ), SLOT( StopEvacuate() ), magicMenu );
        if( entity.Retrieve< Infrastructure_ABC >() )
        {
            AddValuedMagic( magicMenu, menu, tr( "Change Threshold" ), SLOT( ChangeThreshold() ) );
            AddMagic( tr( "Disable" ), SLOT( Disable() ), magicMenu );
            AddMagic( tr( "Enable" ), SLOT( Enable() ), magicMenu );
        }
    }
    else
    {
        AddMagic( tr( "Build" ), SLOT( BuildObject() ), magicMenu );
        AddMagic( tr( "Destroy" ), SLOT( DestroyObject() ), magicMenu );
        if( entity.GetType().CanBeValorized() )
        {
            AddMagic( tr( "Mine" ), SLOT( MineObject() ), magicMenu );
            AddMagic( tr( "Sweep mines" ), SLOT( SweepMineObject() ), magicMenu );
        }
        const Object& obj = static_cast< const Object& >( entity );
        if( obj.IsReservedObstacle() )
        {
            if( obj.IsReservedObstacleActivated() )
                AddMagic( tr( "Deactivate reserved obstacle" ), SLOT( DeactivateReservedObstacle() ), magicMenu );
            else
                AddMagic( tr( "Activate reserved obstacle" ), SLOT( ActivateReservedObstacle() ), magicMenu );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
int ObjectMagicOrdersInterface::AddMagic( const QString& label, const char* slot, QPopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::AddValuedMagic
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::AddValuedMagic( QPopupMenu* parent, kernel::ContextMenu& menu, const QString& label, const char* slot )
{
    QPopupMenu* valueMenu = new QPopupMenu( parent );
    QLineEdit* valueEditor = new InitializedLineEdit( valueMenu, tr( "Enter value" ) );
    valueMenu->insertItem( valueEditor );
    parent->insertItem( label, valueMenu );
    QToolTip::add( valueEditor, tr( "Type-in value then press 'Enter'" ) );
    connect( valueEditor, SIGNAL( returnPressed() ), this, slot );
    connect( valueEditor, SIGNAL( returnPressed() ), menu, SLOT( hide() ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::BuildObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::BuildObject()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Construction", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_construction );
    list.AddIdentifier( "Type", 0 );
    list.AddQuantity( "Number", 0 );
    list.AddNumeric( "Density", 0 );
    list.AddQuantity( "Percentage", 100 );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DestroyObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DestroyObject()
{
    if( !selectedEntity_ )
        return;
    actionsModel_.Publish( *actionsModel_.CreateObjectDestroyMagicAction( *selectedEntity_ ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::MineObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::MineObject()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Mine", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_mine );
    list.AddIdentifier( "Type", 0 );
    list.AddQuantity( "Number", 0 );
    list.AddNumeric( "Density", 0 );
    list.AddQuantity( "Percentage", 100 );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::SweepMineObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::SweepMineObject()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Mine", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_mine );
    list.AddIdentifier( "Type", 0 );
    list.AddQuantity( "Number", 0 );
    list.AddNumeric( "Density", 0 );
    list.AddQuantity( "Percentage", 0 );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::ActivateReservedObstacle
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::ActivateReservedObstacle()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Obstacle", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_obstacle );
    list.AddIdentifier( "TargetType", sword::ObstacleType_DemolitionTargetType_reserved );
    list.AddBool( "Activation", true );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DeactivateReservedObstacle
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DeactivateReservedObstacle()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Obstacle", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_obstacle );
    list.AddIdentifier( "TargetType", sword::ObstacleType_DemolitionTargetType_reserved );
    list.AddBool( "Activation", false );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::ChangeStructuralState
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::ChangeStructuralState()
{
    if( !selectedEntity_ )
        return;
    if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
    {
        ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
        list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_structural_state );
        list.AddNumeric( "Value", 0.01f * editor->text().toInt() );
        actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::Disable
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::Disable()
{
    if( !selectedEntity_ )
        return;
    if( const Infrastructure_ABC* infrastructure = selectedEntity_->Retrieve< Infrastructure_ABC >() )
    {
        ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
        list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_infrastructure );
        list.AddBool( "Enabled", false );
        actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::Enable
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::Enable()
{
    if( !selectedEntity_ )
        return;
    if( const Infrastructure_ABC* infrastructure = selectedEntity_->Retrieve< Infrastructure_ABC >() )
    {
        ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
        list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_infrastructure );
        list.AddBool( "Enabled", true );
        actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::ChangeThreshold
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::ChangeThreshold()
{
    if( !selectedEntity_ )
        return;
    if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        if( const Infrastructure_ABC* infrastructure = selectedEntity_->Retrieve< Infrastructure_ABC >() )
        {
            ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
            list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_infrastructure );
            list.AddNumeric( "Threshold", 0.01f * editor->text().toFloat() );
            actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
        }
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::Alert
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::Alert()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_alerted );
    list.AddBool( "Alerted", true );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::StopAlert
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::StopAlert()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_alerted );
    list.AddBool( "Alerted", false );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::Confine
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::Confine()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_confined );
    list.AddBool( "Confined", true );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::StopConfine
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::StopConfine()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_confined );
    list.AddBool( "Confined", false );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::Evacuate
// Created: ABR 2011-03-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::Evacuate()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_evacuated );
    list.AddBool( "Evacuated", true );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::StopEvacuate
// Created: ABR 2011-03-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::StopEvacuate()
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_evacuated );
    list.AddBool( "Evacuated", false );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}
