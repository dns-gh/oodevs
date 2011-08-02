// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamResourceNetwork.h"
#include "moc_ParamResourceNetwork.cpp"
#include "actions/ParameterContainer_ABC.h"
#include "actions/ResourceNetwork.h"
#include "actions/String.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork constructor
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
ParamResourceNetwork::ParamResourceNetwork( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : QObject       ( parent )
    , controller_   ( controller )
    , Param_ABC     ( parameter.GetName().c_str() )
    , parameter_    ( parameter )
    , current_      ( 0 )
    , selected_     ( 0 )
    , objectLabel_  ( 0 )
    , objectName_   ( 0 )
    , resourceLabel_( 0 )
    , resourceName_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork destructor
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
ParamResourceNetwork::~ParamResourceNetwork()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::BuildInterface
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
QWidget* ParamResourceNetwork::BuildInterface( QWidget* parent )
{
    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, GetName(), parent );
    objectLabel_ = new ::gui::RichLabel( tools::translate( "ParamResourceNetwork", "Object" ), false, group );
    objectName_ = new QLabel( "---", group );
    objectName_->setMinimumWidth( 100 );
    objectName_->setAlignment( Qt::AlignCenter );
    objectName_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    resourceLabel_ = new ::gui::RichLabel( tools::translate( "ParamResourceNetwork", "Resource" ), false, group );
    resourceName_ = new QLabel( "---", group );
    resourceName_->setMinimumWidth( 100 );
    resourceName_->setAlignment( Qt::AlignCenter );
    resourceName_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    return group;
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::CommitTo
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
void ParamResourceNetwork::CommitTo( actions::ParameterContainer_ABC& parameter ) const
{
    if( selected_ )
        parameter.AddParameter( *new actions::parameters::ResourceNetwork( parameter_, *selected_, resourceName_->text().ascii(), controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::MenuItemValidated
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
void ParamResourceNetwork::MenuItemValidated( int index )
{
    selected_ = current_;
    if( selected_ )
    {
        const kernel::ResourceNetwork_ABC& resource = selected_->Get< kernel::ResourceNetwork_ABC >();
        const kernel::ResourceNetwork_ABC::T_ResourceNodes& nodes = resource.GetResourceNodes();
        int i = 0;
        for( kernel::ResourceNetwork_ABC::CIT_ResourceNodes it = nodes.begin(); it != nodes.end(); ++it, ++i )
            if( index == i )
            {
                objectName_->setText( selected_->GetName() );
                resourceName_->setText( it->first.c_str() );
                break;
            }
    }
    else
    {
        objectName_->setText( "---" );
        resourceName_->setText( "---" );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::NotifyContextMenu
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
void ParamResourceNetwork::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    // $$$$ JSR 2011-05-02: TODO gérer Deleted
    current_ = 0;
    const kernel::ResourceNetwork_ABC* resource = entity.Retrieve< kernel::ResourceNetwork_ABC >();
    if( resource )
    {
        const kernel::ResourceNetwork_ABC::T_ResourceNodes& nodes = resource->GetResourceNodes();
        if( nodes.size() > 0 )
        {
            current_ = &entity;
            Q3PopupMenu* popupMenu = new Q3PopupMenu( menu );
            int index = 0;
            for( kernel::ResourceNetwork_ABC::CIT_ResourceNodes it = nodes.begin(); it != nodes.end(); ++it, ++index )
            {
                int id = popupMenu->insertItem( it->first.c_str(), this, SLOT( MenuItemValidated( int ) ) );
                popupMenu->setItemParameter( id, index );
            }
            menu.InsertItem( "Parameter", tools::translate( "ParamResourceNetwork", "Resource network" ), popupMenu );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::Show
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
void ParamResourceNetwork::Show()
{
    objectLabel_->show();
    objectName_->show();
    resourceLabel_->show();
    resourceName_->show();
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::Hide
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
void ParamResourceNetwork::Hide()
{
    objectLabel_->hide();
    objectName_->hide();
    resourceLabel_->hide();
    resourceName_->hide();
}
