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
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Enumeration.h"
#include "actions/Point.h"
#include "actions/Quantity.h"
#include "actions/String.h"
#include "actions/Bool.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/LocationCreator.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_gui/EntityType.h"
#include "gaming/StaticModel.h"
#include "gaming/PopulationDecisions.h"
#include "protocol/SimulationSenders.h"
#include <boost/algorithm/string.hpp>

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
PopulationMagicOrdersInterface::PopulationMagicOrdersInterface( QWidget* parent, Controllers& controllers, ActionsModel& actionsModel, const ::StaticModel& staticModel, const Time_ABC& simulation, ParametersLayer& layer, const Profile_ABC& profile, gui::GlSelector& selector )
    : QObject( parent )
    , controllers_   ( controllers )
    , actionsModel_  ( actionsModel )
    , static_        ( staticModel )
    , simulation_    ( simulation )
    , profile_       ( profile )
    , selector_      ( selector )
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
    kernel::ContextMenu* magicMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ) );
    AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
    const PopulationDecisions* decisions = static_cast< const PopulationDecisions* >( entity.Retrieve< gui::Decisions_ABC >() );
    if( decisions->IsDebugActivated() )
        AddMagic( tr( "Deactivate brain debug" ), SLOT( DeactivateBrainDebug() ), magicMenu );
    else
        AddMagic( tr( "Activate brain debug" ), SLOT( ActivateBrainDebug() ), magicMenu );
    AddReloadBrainMenu( magicMenu, static_.types_.populationModels_,
                        entity.Retrieve< gui::Decisions_ABC >() ? entity.Retrieve< gui::Decisions_ABC >()->GetModelName() : "unknown",
                        entity.Get< gui::EntityType< kernel::PopulationType > >().GetType().GetDecisionalModel().GetName() );
    AddMagic( tr( "Kill all" ), SLOT( KillAllPopulation() ), magicMenu );
    AddValuedMagic( magicMenu, menu, tr( "Change armed individuals:" ), SLOT( ChangeArmedIndividuals() ) );
    kernel::ContextMenu* choiceMenu = new kernel::ContextMenu( magicMenu );
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
            std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
            tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
            action->AddParameter( *new parameters::Point( it.NextElement(), static_.coordinateConverter_, location ) );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
            actionsModel_.Publish( *action );
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
    selector_.SetFocus();
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
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::ReloadBrain
// Created: LDC 2011-08-18
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::ReloadBrain(QAction* action)
{
    if( selectedEntity_ )
    {
        std::string modelName = action->text();
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "reload_brain" );
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::String( it.NextElement(), modelName ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action );
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
            std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
            tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
            action->AddParameter( *new parameters::Quantity( it.NextElement(), editor->text().toInt() ) );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
            actionsModel_.Publish( *action );
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
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::Enumeration( it.NextElement(), index ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
int PopulationMagicOrdersInterface::AddMagic( const QString& label, const char* slot, kernel::ContextMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::AddValuedMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::AddValuedMagic( kernel::ContextMenu* parent, ContextMenu& menu, const QString& label, const char* slot )
{
    kernel::ContextMenu* valueMenu = new kernel::ContextMenu( parent );
    QLineEdit* valueEditor = new InitializedLineEdit( valueMenu, tr( "Enter value" ) );
    QWidgetAction* action = new QWidgetAction( valueMenu );
    action->setDefaultWidget( valueEditor );
    valueMenu->addAction( action );
    parent->insertItem( label, valueMenu );
    QToolTip::add( valueEditor, tr( "Type-in value then press 'Enter'" ) );
    connect( valueEditor, SIGNAL( returnPressed() ), this, slot );
    connect( valueEditor, SIGNAL( returnPressed() ), menu, SLOT( hide() ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::AddReloadBrainMenu
// Created: AHC 2012-01-24
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::AddReloadBrainMenu( QMenu* parent, const tools::StringResolver< DecisionalModel >& models,
    const std::string& currentModel, const std::string& defaultModel)
{
    QMenu* menu = new QMenu( tr( "Reload brain" ), parent );
    tools::Iterator< const kernel::DecisionalModel& > it( models.CreateIterator() );
    menu->addAction( currentModel.c_str() ) ;
    if( defaultModel != currentModel )
        menu->addAction( defaultModel.c_str() ) ;
    std::map<char, QMenu*> subMenus;
    while( it.HasMoreElements() )
    {
        const kernel::DecisionalModel& value = it.NextElement();
        const std::string& name(value.GetName());
        if( name==currentModel || name==defaultModel )
            continue;
        std::string nameCpy(name);
        boost::to_lower( nameCpy );
        char key = nameCpy[0];
        std::map<char, QMenu*>::iterator subIt = subMenus.find(key);
        QMenu* sub = 0;
        if( subMenus.end() == subIt )
        {
            sub = new QMenu( QChar(key), menu);
            menu->addMenu( sub );
            subMenus[key]=sub;
        }
        else
            sub = subIt->second;
        sub->addAction( name.c_str() );
    }
    connect(menu, SIGNAL( triggered(QAction*) ), this, SLOT( ReloadBrain(QAction*) ) );
    parent->addMenu( menu );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::ActivateBrainDebug
// Created: SLI 2013-06-21
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::ActivateBrainDebug()
{
        if( selectedEntity_ )
    {
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_brain_debug" );
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new actions::parameters::Bool( it.NextElement(), true ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::DeactivateBrainDebug
// Created: SLI 2013-06-21
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::DeactivateBrainDebug()
{
    if( selectedEntity_ )
    {
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_brain_debug" );
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new actions::parameters::Bool( it.NextElement(), false ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action );
    }
}
