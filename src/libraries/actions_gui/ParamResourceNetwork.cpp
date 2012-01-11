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
#include "InterfaceBuilder_ABC.h"
#include "actions/ParameterContainer_ABC.h"
#include "actions/ResourceNetwork.h"
#include "actions/String.h"
#include "actions_gui/MissionInterface_ABC.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/Controllers.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork constructor
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
ParamResourceNetwork::ParamResourceNetwork( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC     ( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , controller_   ( builder.GetControllers().controller_ )
    , current_      ( 0 )
    , selected_     ( 0 )
    , objectName_   ( new QLabel() )
    , resourceName_ ( new QLabel() )
{
    OnMenuClick( 0 );
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork destructor
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
ParamResourceNetwork::~ParamResourceNetwork()
{
    delete objectName_;
    delete resourceName_;
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::BuildInterface
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
QWidget* ParamResourceNetwork::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QGridLayout* layout = new QGridLayout( group_ );

    objectName_->setMinimumWidth( 100 );
    objectName_->setAlignment( Qt::AlignCenter );
    objectName_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    resourceName_->setMinimumWidth( 100 );
    resourceName_->setAlignment( Qt::AlignCenter );
    resourceName_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    layout->addWidget( new ::gui::RichLabel( tools::translate( "ParamResourceNetwork", "Object" ), false, parent ), 0, 0 ); // $$$$ ABR 2012-01-05: TODO: Replace RichLabel by QLabel
    layout->addWidget( objectName_, 0, 1 );
    layout->addWidget( new ::gui::RichLabel( tools::translate( "ParamResourceNetwork", "Resource" ), false, parent ), 1, 0 );
    layout->addWidget( resourceName_, 1, 1 );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::CommitTo
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
void ParamResourceNetwork::CommitTo( actions::ParameterContainer_ABC& parameter ) const
{
    if( IsChecked() && selected_ )
        parameter.AddParameter( *new actions::parameters::ResourceNetwork( parameter_, *selected_, resourceName_->text().ascii(), controller_ ) );
    else
        parameter.AddParameter( *new actions::parameters::ResourceNetwork( parameter_, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::CreateInternalMenu
// Created: ABR 2012-01-06
// -----------------------------------------------------------------------------
void ParamResourceNetwork::CreateInternalMenu( kernel::ContextMenu& menu )
{
    kernel::ContextMenu* internalMenu = new kernel::ContextMenu( &menu );
    internalMenu->setTitle( GetMenuName() );
    const kernel::ResourceNetwork_ABC& resource = current_->Get< kernel::ResourceNetwork_ABC >();
    const kernel::ResourceNetwork_ABC::T_ResourceNodes& nodes = resource.GetResourceNodes();
    int index = 0;
    actions_.clear();
    for( kernel::ResourceNetwork_ABC::CIT_ResourceNodes it = nodes.begin(); it != nodes.end(); ++it, ++index )
        actions_.push_back( internalMenu->InsertItem( "", it->first.c_str(), index ) );
    QObject::connect( internalMenu, SIGNAL( triggered( QAction* ) ), this, SLOT( OnMenuClick( QAction* ) ) );
    internalMenu_ = internalMenu;
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::OnMenuClick
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
void ParamResourceNetwork::OnMenuClick( QAction* action )
{
    selected_ = current_;
    if( selected_ )
        for( CIT_Actions it = actions_.begin(); it != actions_.end(); ++it )
        {
            if( *it == action )
            {
                objectName_->setText( selected_->GetName() );
                resourceName_->setText( action->text() );
                break;
            }
        }
    else
    {
        objectName_->setText( "---" );
        resourceName_->setText( "---" );
    }
    if( group_ && IsOptional() )
        group_->setChecked( selected_ != 0 );
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
            Param_ABC::CreateMenu( menu );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetwork::InternalCheckValidity
// Created: ABR 2011-11-22
// -----------------------------------------------------------------------------
bool ParamResourceNetwork::InternalCheckValidity() const
{
    return selected_ != 0;
}
