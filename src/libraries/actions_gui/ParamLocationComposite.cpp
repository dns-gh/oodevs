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
#include "ParamInterface_ABC.h"

#include "actions/Agent.h"
#include "actions/Automat.h"
#include "actions/Location.h"
#include "actions/ObjectKnowledge.h"
#include "actions/Path.h"
#include "actions/Point.h"
#include "actions/Polygon.h"
#include "actions/PopulationKnowledge.h"
#include "actions/UrbanBlock.h"

#include "actions/Parameter.h"
#include "actions/Location.h"

#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/tools.h"
#include "clients_gui/SubObjectName.h"
#include <QtGui/QStackedWidget.h>

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
    : Param_ABC( builder, parameter )
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
Param_ABC* ParamLocationComposite::AddElement( const std::string& type, const std::string& name /* = "" */, bool forceNotOptional /* = false */ )
{
    kernel::OrderParameter orderParam( name.empty() ? GetName().toStdString() : name, type, forceNotOptional ? false : IsOptional() );
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
    for( auto it = params_.begin(); it != params_.end(); ++it )
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
    for( auto it = params_.begin(); it != params_.end(); ++it )
        (*it)->RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::RegisterIn
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void ParamLocationComposite::RegisterIn()
{
    for( auto it = params_.begin(); it != params_.end(); ++it )
        (*it)->RegisterIn();
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Draw
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void ParamLocationComposite::Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( selectedParam_ )
        selectedParam_->Draw( point, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::BuildInterface
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
QWidget* ParamLocationComposite::BuildInterface( const QString& objectName, QWidget* parent )
{
    stack_ = new QStackedWidget();
    parent->layout()->addWidget( stack_ );
    stack_->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed );
    ::gui::SubObjectName subObject( objectName );
    InternalBuildInterface();
    return stack_;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::InternalBuildInterface
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
void ParamLocationComposite::InternalBuildInterface()
{
    if( group_ )
        disconnect( group_, SIGNAL( clicked() ), this, SLOT( Update() ) );
    for( auto it = params_.begin(); it != params_.end(); ++it )
    {
        QWidget* widget = (*it)->BuildInterface( (*it)->GetType().c_str(), stack_ );
        stack_->addWidget( widget );
        connect( static_cast< QGroupBox* >( widget ), SIGNAL( clicked( bool ) ), this, SLOT( OnChecked( bool ) ) );
        widgets_.push_back( widget );
    }

    for( auto it = widgets_.begin(); it != widgets_.end(); ++it )
    {
        if( ( *it )->isEnabled() )
        {
            stack_->setCurrentWidget( *it );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::CheckValidity
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
bool ParamLocationComposite::CheckValidity()
{
    return !IsChecked() || ( selectedParam_ && selectedParam_->CheckValidity() );
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
    auto it = params_.begin();
    auto itWidget = widgets_.begin();
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
        static_cast< QGroupBox* >( *it )->setChecked( checked );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::GetIndex
// Created: ABR 2011-12-29
// -----------------------------------------------------------------------------
int ParamLocationComposite::GetIndex( Param_ABC* param ) const
{
    for( size_t i = 0; i < params_.size(); ++i )
        if( params_[ i ] == param )
            return static_cast< int >( i );
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
// Name: ParamLocationComposite::SetName
// Created: ABR 2012-02-21
// -----------------------------------------------------------------------------
void ParamLocationComposite::SetName( const QString& name )
{
    Param_ABC::SetName( name );
    for( IT_Params it = params_.begin(); it != params_.end(); ++it )
        ( *it )->SetName( name );
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

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void ParamLocationComposite::SetEntity( const kernel::Entity_ABC* entity )
{
    for( auto it = params_.begin(); it != params_.end(); ++it )
        ( *it )->SetEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Visit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void ParamLocationComposite::Visit( const actions::parameters::Agent& param )
{
    InternalVisit( param, param.IsKnowledge() ? "agentknowledge" : "agent" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Visit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void ParamLocationComposite::Visit( const actions::parameters::Automat& param )
{
    InternalVisit( param, "automat" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Visit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void ParamLocationComposite::Visit( const actions::parameters::Location& param )
{
    InternalVisit( param, tools::ToString( param.GetLocationType() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Visit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void ParamLocationComposite::Visit( const actions::parameters::ObjectKnowledge& param )
{
    InternalVisit( param, "objectknowledge" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Visit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void ParamLocationComposite::Visit( const actions::parameters::Path& param )
{
    InternalVisit( param, "path" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Visit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void ParamLocationComposite::Visit( const actions::parameters::Point& param )
{
    InternalVisit( param, "point" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Visit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void ParamLocationComposite::Visit( const actions::parameters::Polygon& param )
{
    InternalVisit( param, "polygon" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Visit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void ParamLocationComposite::Visit( const actions::parameters::PopulationKnowledge& param )
{
    InternalVisit( param, "crowdknowledge" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::Visit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void ParamLocationComposite::Visit( const actions::parameters::UrbanBlock& param )
{
    InternalVisit( param, "urbanknowledge" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::InternalVisit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
template< typename T >
void ParamLocationComposite::InternalVisit( const T& param, const std::string& type )
{
    for( auto it = params_.begin(); it != params_.end(); ++it )
    {
        Param_ABC* internalParam = *it;
        if( !internalParam )
            continue;
        if( internalParam->GetType() == type )
        {
            param.Accept( *internalParam );
            NotifyChanged( *internalParam );
            Update();
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::HasParameter
// Created: ABR 2013-11-14
// -----------------------------------------------------------------------------
bool ParamLocationComposite::HasParameter( const Param_ABC& param ) const
{
    for( auto it = params_.begin(); it != params_.end(); ++it )
        if( ( *it )->HasParameter( param ) )
            return true;
    return Param_ABC::HasParameter( param );
}
