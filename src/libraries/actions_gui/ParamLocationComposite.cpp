// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamLocationComposite.h"
#include "moc_ParamLocationComposite.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Parameter.h"
#include "actions/Location.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/tools.h"
#include <QtGui/QStackedWidget.h>
#include "MissionInterface_ABC.h"

using namespace actions::gui;

namespace
{
    class ParameterChoiceVisitor : public kernel::ChoicesVisitor_ABC, private boost::noncopyable
    {
    public:
        ParameterChoiceVisitor( ParamLocationComposite& paramcomposite )
            : paramcomposite_( paramcomposite )
        {}
        virtual void Visit( const std::string& type )
        {
            paramcomposite_.AddElement( type );
        }
    private:
        ParamLocationComposite& paramcomposite_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite constructor
// Created: ABR 2011-12-29
// -----------------------------------------------------------------------------
ParamLocationComposite::ParamLocationComposite( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC ( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , builder_      ( builder )
    , selectedParam_( 0 )
    , stack_        ( 0 )
{
    ParameterChoiceVisitor visitor( *this );
    parameter.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite destructor
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
ParamLocationComposite::~ParamLocationComposite()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::AddElement
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
Param_ABC* ParamLocationComposite::AddElement( const std::string& type, const std::string& name /*= ""*/ )
{
    kernel::OrderParameter orderParam( name.empty() ? GetName().toStdString() : name, type, IsOptional() );
    Param_ABC* param = &builder_.BuildOne( orderParam, false );
    param->SetType( type );
    params_.push_back( param );
    param->RegisterListener( *this );
    return param;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Purge
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
void ParamLocationComposite::Purge()
{
    RemoveFromController();
    for( CIT_Params it = params_.begin(); it != params_.end(); ++it )
        delete *it;
    params_.clear();
    for( std::vector< QWidget* >::const_iterator it = widgets_.begin(); it != widgets_.end(); ++it )
    {
        stack_->removeWidget( *it );
        delete *it;
    }
    widgets_.clear();
    selectedParam_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::RemoveFromController
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void ParamLocationComposite::RemoveFromController()
{
    for( CIT_Params it = params_.begin(); it != params_.end(); ++it )
        (*it)->RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::RegisterIn
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void ParamLocationComposite::RegisterIn( kernel::ActionController& controller )
{
    for( CIT_Params it = params_.begin(); it != params_.end(); ++it )
        (*it)->RegisterIn( controller );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Draw
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void ParamLocationComposite::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( selectedParam_ )
        selectedParam_->Draw( point, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::BuildInterface
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
QWidget* ParamLocationComposite::BuildInterface( QWidget* parent )
{
    stack_ = new QStackedWidget( parent );
    InternalBuildInterface();
    return stack_;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::InternalBuildInterface
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
void ParamLocationComposite::InternalBuildInterface()
{
    for( CIT_Params it = params_.begin(); it != params_.end(); ++it )
    {
        QWidget* widget = (*it)->BuildInterface( stack_ );
        stack_->addWidget( widget );
        connect( static_cast< QGroupBox* >( widget ), SIGNAL( clicked( bool ) ), this, SLOT( OnChecked( bool ) ) );
        widgets_.push_back( widget );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::CheckValidity
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
bool ParamLocationComposite::CheckValidity()
{
    if( IsChecked() && ( !selectedParam_ || !selectedParam_->CheckValidity() ) )
    {
        for( std::vector< QWidget* >::iterator it = widgets_.begin(); it != widgets_.end(); ++it )
            static_cast< ::gui::RichGroupBox* >( *it )->Warn();
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::CommitTo
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void ParamLocationComposite::CommitTo( actions::ParameterContainer_ABC& container ) const
{
    if( selectedParam_ )
        selectedParam_->CommitTo( container );
    else
        container.AddParameter( *new actions::parameters::Location( parameter_, builder_.GetStaticModel().coordinateConverter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::NotifyChanged
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void ParamLocationComposite::NotifyChanged( Param_ABC& param )
{
    selectedParam_ = &param;
    assert( widgets_.size() == params_.size() );
    std::vector<Param_ABC*>::const_iterator it = params_.begin();
    std::vector<QWidget*>::const_iterator itWidget = widgets_.begin();
    for( ; it != params_.end(); ++it, ++itWidget )
    {
        if( selectedParam_ == *it )
        {
            stack_->setCurrentWidget( *itWidget );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::IsChecked
// Created: ABR 2011-11-22
// -----------------------------------------------------------------------------
bool ParamLocationComposite::IsChecked() const
{
    if( IsOptional() && !widgets_.empty() )
        return static_cast< const QGroupBox* >( widgets_[ 0 ] )->isChecked();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::OnChecked
// Created: ABR 2011-11-22
// -----------------------------------------------------------------------------
void ParamLocationComposite::OnChecked( bool checked )
{
    for( std::vector< QWidget* >::iterator it = widgets_.begin(); it != widgets_.end(); ++it )
    {
        disconnect( static_cast< QGroupBox* >( *it ), SIGNAL( clicked( bool ) ), this, SLOT( OnChecked( bool ) ) );
        static_cast< QGroupBox* >( *it )->setChecked( checked );
        connect( static_cast< QGroupBox* >( *it ), SIGNAL( clicked( bool ) ), this, SLOT( OnChecked( bool ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::GetIndex
// Created: ABR 2011-12-29
// -----------------------------------------------------------------------------
int ParamLocationComposite::GetIndex( Param_ABC* param ) const
{
    for( int i = 0; i < params_.size(); ++i )
        if( params_[ i ] == param )
            return i;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::SetParentList
// Created: ABR 2011-12-29
// -----------------------------------------------------------------------------
void ParamLocationComposite::SetParentList( ListParameterBase* parentList )
{
    Param_ABC::SetParentList( parentList );
    for( IT_Params it = params_.begin(); it != params_.end(); ++it )
        ( *it )->SetParentList( parentList );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::CreateInternalMenu
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
void ParamLocationComposite::CreateInternalMenu( kernel::ContextMenu& mainMenu )
{
    kernel::ContextMenu* menu = new kernel::ContextMenu( &mainMenu );
    menu->setTitle( GetMenuName() );
    internalMenu_ = menu;
}
