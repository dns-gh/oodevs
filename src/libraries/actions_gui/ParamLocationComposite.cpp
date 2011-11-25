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
#include "actions/Parameter_ABC.h"
#include "actions/Parameter.h"
#include "actions/Location.h"
#include <Qt3Support/Q3widgetstack.h>

class QWidget;

using namespace actions::gui;

namespace
{
    class ChoiceVisitor : public kernel::ChoicesVisitor_ABC
    {
    public:
        ChoiceVisitor( Param_ABC& paramcomposite, const std::string& name, std::vector<Param_ABC*>& parameters, const InterfaceBuilder_ABC& builder )
            : paramcomposite_( paramcomposite )
            , name_          ( name )
            , parameters_    ( parameters )
            , builder_       ( builder )
        {}
        virtual void Visit( const std::string& type )
        {
            kernel::OrderParameter orderParam( name_, type, paramcomposite_.IsOptional() );
            Param_ABC* param = &builder_.Build( orderParam, false );
            parameters_.push_back( param );
            param->RegisterListener( paramcomposite_ );
        }
    private:
        ChoiceVisitor( const ChoiceVisitor& );
        ChoiceVisitor& operator=( const ChoiceVisitor& );
    private:
        Param_ABC& paramcomposite_;
        std::string name_;
        std::vector<Param_ABC*>& parameters_;
        const InterfaceBuilder_ABC& builder_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite constructor
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
ParamLocationComposite::ParamLocationComposite( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const InterfaceBuilder_ABC& builder )
    : Param_ABC( parameter.GetName().c_str(), parameter.IsOptional() )
    , parameter_( parameter )
    , converter_( converter )
    , selectedParam_( 0 )
{
    ChoiceVisitor visitor( *this, GetName().ascii(), params_, builder );
    parameter.Accept( visitor );
}
// -----------------------------------------------------------------------------
// Name: ParamLocationComposite destructor
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
ParamLocationComposite::~ParamLocationComposite()
{
    RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::RemoveFromController
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void ParamLocationComposite::RemoveFromController()
{
    for( CIT_Params it = params_.begin(); it != params_.end(); ++it )
        (*it)->RemoveFromController();
    Param_ABC::RemoveFromController();
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
    stack_ = new Q3WidgetStack( parent );
    for( CIT_Params it = params_.begin(); it != params_.end(); ++it )
    {
        QWidget* widget = (*it)->BuildInterface( stack_ );
        connect( static_cast< QGroupBox* >( widget ), SIGNAL( clicked( bool ) ), this, SLOT( OnChecked( bool ) ) );
        widgets_.push_back( widget );
    }
    return stack_;
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
        container.AddParameter( *new actions::parameters::Location( parameter_, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::NotifyChanged
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void ParamLocationComposite::NotifyChanged( Param_ABC& param )
{
    selectedParam_ = &param;
    std::vector<Param_ABC*>::const_iterator it = params_.begin();
    std::vector<QWidget*>::const_iterator itWidget = widgets_.begin();
    for( ; it != params_.end(); ++it, ++itWidget )
    {
        if( selectedParam_ == *it )
        {
            stack_->raiseWidget( *itWidget );
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
