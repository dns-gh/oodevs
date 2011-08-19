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
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Enumeration.h"
#include "actions/Point.h"
#include "actions/Quantity.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/LocationCreator.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "gaming/StaticModel.h"
#include "gaming/tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;
using namespace gui;
using namespace actions;

namespace
{
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
// Name: PopulationMagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
PopulationMagicOrdersInterface::PopulationMagicOrdersInterface( QWidget* parent, Controllers& controllers, ActionsModel& actionsModel, const ::StaticModel& staticModel, const Time_ABC& simulation, ParametersLayer& layer, const Profile_ABC& profile )
    : QObject( parent )
    , controllers_   ( controllers )
    , actionsModel_  ( actionsModel )
    , static_        ( staticModel )
    , simulation_    ( simulation )
    , profile_       ( profile )
    , selectedEntity_( controllers )
    , magicMove_     ( false )
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
    Q3PopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
    AddMagic( tr( "Reload brain" ), SLOT( ReloadBrain() ), magicMenu );
    AddMagic( tr( "Kill all" ), SLOT( KillAllPopulation() ), magicMenu );
    AddValuedMagic( magicMenu, menu, tr( "Change armed individuals:" ), SLOT( ChangeArmedIndividuals() ) );
    Q3PopupMenu* choiceMenu = new Q3PopupMenu( magicMenu );
    for( unsigned int i = 0; i < unsigned int( eNbrPopulationAttitude ); ++i )
        choiceMenu->insertItem( tools::ToString( static_cast< E_PopulationAttitude >( i ) ), this, SLOT( ChangePopulationAttitude( int ) ), 0, i );
    magicMenu->insertItem( tr( "Change crowd attitude" ), choiceMenu );
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
            // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
            MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "teleport" );
            UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, tr( "Teleport" ), true );
            tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
            action->AddParameter( *new parameters::Point( it.NextElement(), static_.coordinateConverter_, location ) );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new ActionTasker( selectedEntity_, false ) );
            action->RegisterAndPublish( actionsModel_ );
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
    if( !magicMove_ )
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
        // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "crowd_total_destruction" );
        UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, tr( "Crowd Total Destruction" ), true );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( selectedEntity_, false ) );
        action->RegisterAndPublish( actionsModel_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::ReloadBrain
// Created: LDC 2011-08-18
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::ReloadBrain()
{
    if( selectedEntity_ )
    {
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "reload_brain" );
        UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, tr( "Reload brain" ), true );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( selectedEntity_, false ) );
        action->RegisterAndPublish( actionsModel_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::ChangeArmedIndividuals
// Created: JSR 2011-03-16
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::ChangeArmedIndividuals()
{
    if( selectedEntity_ )
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        {
            // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
            MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "crowd_change_armed_individuals" );
            UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, tr( "Crowd Change Armed Individuals" ), true );
            tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
            action->AddParameter( *new parameters::Quantity( it.NextElement(), editor->text().toInt() ) );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new ActionTasker( selectedEntity_, false ) );
            action->RegisterAndPublish( actionsModel_ );
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
        // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "crowd_change_attitude" );
        UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, tr( "Crowd Change Attitude" ), true );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::Enumeration( it.NextElement(), index ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( selectedEntity_, false ) );
        action->RegisterAndPublish( actionsModel_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
int PopulationMagicOrdersInterface::AddMagic( const QString& label, const char* slot, Q3PopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::AddValuedMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::AddValuedMagic( Q3PopupMenu* parent, ContextMenu& menu, const QString& label, const char* slot )
{
    Q3PopupMenu* valueMenu = new Q3PopupMenu( parent );
    QLineEdit* valueEditor = new InitializedLineEdit( valueMenu, tr( "Enter value" ) );
    valueMenu->insertItem( valueEditor->text() );
    parent->insertItem( label, valueMenu );
    QToolTip::add( valueEditor, tr( "Type-in value then press 'Enter'" ) );
    connect( valueEditor, SIGNAL( returnPressed() ), this, slot );
    connect( valueEditor, SIGNAL( returnPressed() ), menu, SLOT( hide() ) );
}
