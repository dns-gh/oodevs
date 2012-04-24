// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamResourceNetworkNode.h"
#include "moc_ParamResourceNetworkNode.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/ParameterContainer_ABC.h"
#include "actions/ResourceNetworkNode.h"
#include "actions/String.h"
#include "actions_gui/MissionInterface_ABC.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/Controllers.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkNode constructor
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
ParamResourceNetworkNode::ParamResourceNetworkNode( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
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
// Name: ParamResourceNetworkNode destructor
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
ParamResourceNetworkNode::~ParamResourceNetworkNode()
{
    delete objectName_;
    delete resourceName_;
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkNode::BuildInterface
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
QWidget* ParamResourceNetworkNode::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QGridLayout* layout = new QGridLayout( group_ );

    objectName_->setMinimumWidth( 100 );
    objectName_->setAlignment( Qt::AlignCenter );
    objectName_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    resourceName_->setMinimumWidth( 100 );
    resourceName_->setAlignment( Qt::AlignCenter );
    resourceName_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    layout->addWidget( new ::gui::RichLabel( tools::translate( "ParamResourceNetworkNode", "Object" ), false, parent ), 0, 0 ); // $$$$ ABR 2012-01-05: TODO: Replace RichLabel by QLabel
    layout->addWidget( objectName_, 0, 1 );
    layout->addWidget( new ::gui::RichLabel( tools::translate( "ParamResourceNetworkNode", "Resource" ), false, parent ), 1, 0 );
    layout->addWidget( resourceName_, 1, 1 );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkNode::CommitTo
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
void ParamResourceNetworkNode::CommitTo( actions::ParameterContainer_ABC& parameter ) const
{
    if( IsChecked() && selected_ )
        parameter.AddParameter( *new actions::parameters::ResourceNetworkNode( parameter_, *selected_, resourceName_->text().ascii(), controller_ ) );
    else
        parameter.AddParameter( *new actions::parameters::ResourceNetworkNode( parameter_, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkNode::CreateInternalMenu
// Created: ABR 2012-01-06
// -----------------------------------------------------------------------------
void ParamResourceNetworkNode::CreateInternalMenu( kernel::ContextMenu& menu )
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
// Name: ParamResourceNetworkNode::OnMenuClick
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
void ParamResourceNetworkNode::OnMenuClick( QAction* action )
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
// Name: ParamResourceNetworkNode::NotifyContextMenu
// Created: JSR 2011-05-02
// -----------------------------------------------------------------------------
void ParamResourceNetworkNode::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    // $$$$ JSR 2011-05-02: TODO g�rer Deleted
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
// Name: ParamResourceNetworkNode::InternalCheckValidity
// Created: ABR 2011-11-22
// -----------------------------------------------------------------------------
bool ParamResourceNetworkNode::InternalCheckValidity() const
{
    return selected_ != 0;
}

