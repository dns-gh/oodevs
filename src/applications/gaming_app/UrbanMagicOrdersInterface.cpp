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
#include "actions/UrbanMagicAction.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "gaming/StaticModel.h"
#include "gaming/MagicOrders.h"
#include "protocol/SimulationSenders.h"
#include <google/protobuf/Message.h>
#include <google/protobuf/Descriptor.h>

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
// Name: UrbanMagicOrdersInterface constructor
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
UrbanMagicOrdersInterface::UrbanMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation )
    : QObject( parent )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , selectedEntity_( controllers )
{
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface destructor
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
UrbanMagicOrdersInterface::~UrbanMagicOrdersInterface()
{
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::NotifyContextMenu
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::NotifyContextMenu( const kernel::Object_ABC& object, kernel::ContextMenu& menu )
{
    selectedEntity_ = &object;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    AddValuedMagic( magicMenu, menu, tr( "Change Urban state" ), SLOT( ChangeStructuralState() ) );
    const kernel::Infrastructure_ABC* infra = object.Retrieve< kernel::Infrastructure_ABC >();
    if( infra )
    {
        AddValuedMagic( magicMenu, menu, tr( "Change Threshold" ), SLOT( ChangeThreshold() ) );
        if( infra->IsEnabled() )
            AddMagic( tr( "Disable" ), SLOT( Disable() ), magicMenu );
        else
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
void UrbanMagicOrdersInterface::AddValuedMagic( QPopupMenu* parent, kernel::ContextMenu& menu, const QString& label, const char* slot )
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
    {
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        {
            actions::UrbanMagicAction* action = new actions::UrbanMagicAction( *selectedEntity_, controllers_.controller_, true );
            action->SetStructuralState( editor->text().toInt() );
            SendUrbanUpdateMagic( *action );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::Disable
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::Disable()
{
    if( selectedEntity_ )
    {
        actions::UrbanMagicAction* action = new actions::UrbanMagicAction( *selectedEntity_, controllers_.controller_, true );
        action->SetEnabled( false );
        SendUrbanUpdateMagic( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::Enable
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::Enable()
{
    if( selectedEntity_ )
    {
        actions::UrbanMagicAction* action = new actions::UrbanMagicAction( *selectedEntity_, controllers_.controller_, true );
        action->SetEnabled( true );
        SendUrbanUpdateMagic( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::ChangeThreshold
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::ChangeThreshold()
{
    if( selectedEntity_ )
    {
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        {
            actions::UrbanMagicAction* action = new actions::UrbanMagicAction( *selectedEntity_, controllers_.controller_, true );
            action->SetThreshold( editor->text().toFloat() );
            SendUrbanUpdateMagic( *action );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicOrdersInterface::SendUrbanUpdateMagic
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicOrdersInterface::SendUrbanUpdateMagic( actions::UrbanMagicAction& action )
{
    action.Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action.Attach( *new actions::ActionTasker( selectedEntity_, false ) );
    action.RegisterAndPublish( actionsModel_ );
}
