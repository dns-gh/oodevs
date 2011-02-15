// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UrbanMagicOrdersInterface.h"
#include "moc_UrbanMagicOrdersInterface.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/ObjectMagicAction.h"
#include "actions/ParameterList.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "gaming/StaticModel.h"
#include "gaming/MagicOrders.h"
#include "protocol/SimulationSenders.h"

using namespace actions;
using namespace kernel;
using namespace parameters;

// $$$$ _RC_ JSR 2011-02-11: TODO A unifier avec ObjectMagicOrdersInterface

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
// Name: UrbanMagicOrdersInterface constructor
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
UrbanMagicOrdersInterface::UrbanMagicOrdersInterface( QWidget* parent, Controllers& controllers, ActionsModel& actionsModel, const ::StaticModel& staticModel, const Time_ABC& simulation )
    : QObject( parent )
    , controllers_   ( controllers )
    , actionsModel_  ( actionsModel )
    , static_        ( staticModel )
    , simulation_    ( simulation )
    , selectedEntity_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface destructor
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
UrbanMagicOrdersInterface::~UrbanMagicOrdersInterface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::NotifyContextMenu
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::NotifyContextMenu( const Object_ABC& object, ContextMenu& menu )
{
    selectedEntity_ = &object;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    AddValuedMagic( magicMenu, menu, tr( "Change Urban state" ), SLOT( ChangeStructuralState() ) );
    if( object.Retrieve< Infrastructure_ABC >() )
    {
        AddValuedMagic( magicMenu, menu, tr( "Change Threshold" ), SLOT( ChangeThreshold() ) );
        AddMagic( tr( "Disable" ), SLOT( Disable() ), magicMenu );
        AddMagic( tr( "Enable" ), SLOT( Enable() ), magicMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::AddMagic
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::AddMagic( const QString& label, int id, QPopupMenu* menu )
{
    const int nId = menu->insertItem( label, this, SLOT( Magic( int ) ) );
    menu->setItemParameter( nId, id );
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::AddMagic
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
int UrbanMagicOrdersInterface::AddMagic( const QString& label, const char* slot, QPopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::AddValuedMagic
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::AddValuedMagic( QPopupMenu* parent, ContextMenu& menu, const QString& label, const char* slot )
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
// Name: UrbanMagicOrdersInterface::ChangeStructuralState
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::ChangeStructuralState()
{
    if( selectedEntity_ )
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        {
            ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
            list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_structural_state );
            list.AddNumeric( "Value", 0.01f * editor->text().toInt() );
            SendUrbanUpdateMagic( list );
        }
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::Disable
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::Disable()
{
    if( selectedEntity_ )
        if( const Infrastructure_ABC* infrastructure = selectedEntity_->Retrieve< Infrastructure_ABC >() )
        {
            ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
            list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_infrastructure );
            list.AddBool( "Enabled", false );
            SendUrbanUpdateMagic( list );
        }
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::Enable
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::Enable()
{
    if( selectedEntity_ )
        if( const Infrastructure_ABC* infrastructure = selectedEntity_->Retrieve< Infrastructure_ABC >() )
        {
            ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
            list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_infrastructure );
            list.AddBool( "Enabled", true );
            SendUrbanUpdateMagic( list );
        }
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::ChangeThreshold
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::ChangeThreshold()
{
    if( selectedEntity_ )
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
            if( const Infrastructure_ABC* infrastructure = selectedEntity_->Retrieve< Infrastructure_ABC >() )
            {
                ParameterList& list = *new ParameterList( OrderParameter( "Structural", "list", false ) );
                list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_infrastructure );
                list.AddNumeric( "Threshold", 0.01f * editor->text().toFloat() );
                SendUrbanUpdateMagic( list );
            }
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::SendUrbanUpdateMagic
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::SendUrbanUpdateMagic( ParameterList& attribute )
{
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "update_object" );
    ObjectMagicAction* action = new ObjectMagicAction( selectedEntity_, actionType, controllers_.controller_, true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    ParameterList* attributesList = new ParameterList( it.NextElement() );
    action->AddParameter( *attributesList );
    attributesList->AddParameter( attribute );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    //action.Attach( *new ActionTasker( selectedEntity_, false ) );
    action->RegisterAndPublish( actionsModel_ );
}
