// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationMagicOrdersInterface.h"
#include "moc_PopulationMagicOrdersInterface.cpp"

#include "actions/UnitMagicAction.h"
#include "actions/Point.h"
#include "actions/Quantity.h"
#include "actions/Enumeration.h"

#include "clients_gui/LocationCreator.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Point.h"

#include "gaming/ActionPublisher.h"
#include "gaming/ActionTiming.h"
#include "gaming/StaticModel.h"
#include "gaming/tools.h"
#include "protocol/simulationsenders.h"


using namespace Common;
using namespace kernel;
using namespace gui;
using namespace actions;

namespace
{
    class InitializedLineEdit : public QLineEdit
    {
    public:
        InitializedLineEdit( QWidget* parent, const QString& initialValue )
            : QLineEdit( initialValue, parent ), initialValue_( initialValue ) {}

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
// Name: PopulationMagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
PopulationMagicOrdersInterface::PopulationMagicOrdersInterface( QWidget* parent, Controllers& controllers, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation, ParametersLayer& layer, const Profile_ABC& profile )
    : QObject( parent )
    , controllers_( controllers )
    , actionPublisher_( actionPublisher )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , profile_( profile )
    , selectedEntity_( controllers )
    , magicMove_( false )
{
    magicMoveLocation_ = new LocationCreator( 0, layer, *this );
    magicMoveLocation_->Allow( false, false, false, false, false );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
PopulationMagicOrdersInterface::~PopulationMagicOrdersInterface()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::NotifyContextMenu( const Population_ABC& entity, ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) )
        return;

    selectedEntity_ = &entity;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
    AddMagic( tr( "Kill all" ), SLOT( KillAllPopulation() ), magicMenu );
    AddValuedMagic( magicMenu, menu, tr( "Kill people:" ), SLOT( KillSomePopulation() ) );
    AddValuedMagic( magicMenu, menu, tr( "Resurect people:" ), SLOT( ResurectSomePopulation() ) );

    QPopupMenu* choiceMenu = new QPopupMenu( magicMenu );
    for( unsigned int i = 0; i < unsigned int( eNbrPopulationAttitude ); ++i )
        choiceMenu->insertItem( tools::ToString( (E_PopulationAttitude)i ), this, SLOT( ChangePopulationAttitude( int ) ), 0, i );
    magicMenu->insertItem( tr( "Change population attitude" ), choiceMenu );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::Handle
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::Handle( Location_ABC& location )
{
    if( magicMove_ && location.IsValid() )
    {
        if( selectedEntity_ )
        {
            MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "teleport" );
            UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, true );
            tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
            action->AddParameter( *new parameters::Point( it.NextElement(), static_.coordinateConverter_, location ) );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
            action->RegisterAndPublish( actionsModel_, actionPublisher_ );
        }
    }
    controllers_.Unregister( *magicMoveLocation_ );
    magicMove_ = false;
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::Move
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::Move()
{
    if( magicMove_ )
        return;
    controllers_.Register( *magicMoveLocation_ );
    magicMove_ = true;
    magicMoveLocation_->StartPoint();
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::KillAllPopulation
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::KillAllPopulation()
{
    if( selectedEntity_ )
    {
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "population_total_destruction" );
        UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, true );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
        action->RegisterAndPublish( actionsModel_, actionPublisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::KillSomePopulation
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::KillSomePopulation()
{
    if( selectedEntity_ )
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        {
            MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "population_kill" );
            UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, true );
            tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
            action->AddParameter( *new parameters::Quantity( it.NextElement(), editor->text().toInt() ) );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
            action->RegisterAndPublish( actionsModel_, actionPublisher_ );
        }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::ResurectSomePopulation
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::ResurectSomePopulation()
{
    if( selectedEntity_ )
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        {
            MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "population_resurrect" );
            UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, true );
            tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
            action->AddParameter( *new parameters::Quantity( it.NextElement(), editor->text().toInt() ) );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
            action->RegisterAndPublish( actionsModel_, actionPublisher_ );
        }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::ChangePopulationAttitude
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::ChangePopulationAttitude( int index )
{
    if( selectedEntity_ )
    {
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "population_change_attitude" );
        UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, true );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::Enumeration( it.NextElement(), index ) );
        // $$$$ JSR 2010-04-16: TODO? not used by now
        // optional int32 flux
        // optional int32 concentration
        // optional bool global
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
        action->RegisterAndPublish( actionsModel_, actionPublisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
int PopulationMagicOrdersInterface::AddMagic( const QString& label, const char* slot, QPopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::AddValuedMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::AddValuedMagic( QPopupMenu* parent, ContextMenu& menu, const QString& label, const char* slot )
{
    QPopupMenu* valueMenu = new QPopupMenu( parent );
    QLineEdit* valueEditor = new InitializedLineEdit( valueMenu, tr( "Enter value" ) );
    valueMenu->insertItem( valueEditor );
    parent->insertItem( label, valueMenu );
    QToolTip::add( valueEditor, tr( "Type-in value then press 'Enter'" ) );
    connect( valueEditor, SIGNAL( returnPressed() ), this, slot );
    connect( valueEditor, SIGNAL( returnPressed() ), menu, SLOT( hide() ) );
}
