// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
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
#include "gaming/FloodAttribute.h"
#include "gaming/Object.h"
#include "protocol/SimulationSenders.h"

using namespace actions;
using namespace kernel;
using namespace parameters;

namespace
{
    // $$$$ _RC_ JSR 2011-03-01: Commun avec PopulationMagicOrdersInterface -> à mettre dans un fichier commun
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
            AddMagic( tr( "Disable" ), SLOT( DisableInfrastructure() ), magicMenu );
            AddMagic( tr( "Enable" ), SLOT( EnableInfrastructure() ), magicMenu );
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
        if( entity.Retrieve< FloodAttribute_ABC >() )
            AddMagic( tr( "Generate flood" ), SLOT( GenerateFlood() ), magicMenu );
        const kernel::ObstacleAttribute_ABC* obstacle = entity.Retrieve< kernel::ObstacleAttribute_ABC >();
        if( obstacle && obstacle->IsReservedObstacle() )
        {
            if( obstacle->IsReservedObstacleActivated() )
                AddMagic( tr( "Deactivate reserved obstacle" ), SLOT( DeactivateReservedObstacle() ), magicMenu );
            else
                AddMagic( tr( "Activate reserved obstacle" ), SLOT( ActivateReservedObstacle() ), magicMenu );
        }
        if( const kernel::UndergroundAttribute_ABC* underground = entity.Retrieve< kernel::UndergroundAttribute_ABC >() )
        {
            if( underground->IsActivated() )
                AddMagic( tr( "Deactivate exit" ), SLOT( DeactivateUndergroundExit() ), magicMenu );
            else
                AddMagic( tr( "Activate exit" ), SLOT( ActivateUndergroundExit() ), magicMenu );
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
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::construction );
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
// Name: ObjectMagicOrdersInterface::DoMineObject
// Created: JSR 2011-07-13
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DoMineObject( int quantity )
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Mine", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::mine );
    list.AddIdentifier( "Type", 0 );
    list.AddQuantity( "Number", 0 );
    list.AddNumeric( "Density", 0 );
    list.AddQuantity( "Percentage", quantity );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::MineObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::MineObject()
{
    DoMineObject( 100 );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::SweepMineObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::SweepMineObject()
{
    DoMineObject( 0 );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DoActivateReservedObstacle
// Created: JSR 2011-07-13
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DoActivateReservedObstacle( bool activate )
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Obstacle", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::obstacle );
    list.AddIdentifier( "TargetType", sword::ObstacleType::reserved );
    list.AddBool( "Activation", activate );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::ActivateReservedObstacle
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::ActivateReservedObstacle()
{
    DoActivateReservedObstacle( true );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DeactivateReservedObstacle
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DeactivateReservedObstacle()
{
    DoActivateReservedObstacle( false );
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
        list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::structural_state );
        list.AddNumeric( "Value", 0.01f * editor->text().toInt() );
        actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::PublishActivation
// Created: JSR 2011-07-18
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::PublishActivation( const std::string& name, unsigned int id, bool activate )
{
    if( !selectedEntity_ )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( name, "list", false ) );
    list.AddIdentifier( "AttributeId", id );
    list.AddBool( name, activate );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DisableInfrastructure
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DisableInfrastructure()
{
    PublishActivation( "Infrastructure", sword::ObjectMagicAction::infrastructure, false );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::EnableInfrastructure
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::EnableInfrastructure()
{
    PublishActivation( "Infrastructure", sword::ObjectMagicAction::infrastructure, true );
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
            ParameterList& list = *new ParameterList( OrderParameter( "Infrastructure", "list", false ) );
            list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::infrastructure );
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
    PublishActivation( "Alerted", sword::ObjectMagicAction::alerted, true );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::StopAlert
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::StopAlert()
{
    PublishActivation( "Alerted", sword::ObjectMagicAction::alerted, false );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::Confine
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::Confine()
{
    PublishActivation( "Confined", sword::ObjectMagicAction::confined, true );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::StopConfine
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::StopConfine()
{
    PublishActivation( "Confined", sword::ObjectMagicAction::confined, false );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::Evacuate
// Created: ABR 2011-03-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::Evacuate()
{
    PublishActivation( "Evacuated", sword::ObjectMagicAction::evacuated, true );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::StopEvacuate
// Created: ABR 2011-03-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::StopEvacuate()
{
    PublishActivation( "Evacuated", sword::ObjectMagicAction::evacuated, false );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::GenerateFlood
// Created: JSR 2011-05-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::GenerateFlood()
{
    if( !selectedEntity_ )
        return;
    FloodAttribute* flood = static_cast< FloodAttribute* >( selectedEntity_.ConstCast()->Retrieve< FloodAttribute_ABC >() );
    if( !flood )
        return;
    ParameterList& list = *new ParameterList( OrderParameter( "Flood", "list", false ) );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::flood );
    actionsModel_.Publish( *actionsModel_.CreateObjectUpdateMagicAction( *selectedEntity_, list ) );
    flood->GenerateFlood( true );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::ActivateUndergroundExit
// Created: JSR 2011-07-13
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::ActivateUndergroundExit()
{
    PublishActivation( "Underground", sword::ObjectMagicAction::underground, true );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DeactivateUndergroundExit
// Created: JSR 2011-07-13
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DeactivateUndergroundExit()
{
    PublishActivation( "Underground", sword::ObjectMagicAction::underground, false );
}
