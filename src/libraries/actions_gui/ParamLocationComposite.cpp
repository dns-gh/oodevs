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
#include "InterfaceBuilder_ABC.h"
#include <Qt/QWidgetStack.h>
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
            kernel::OrderParameter orderParam( name_, type, true );
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
ParamLocationComposite::ParamLocationComposite( const kernel::OrderParameter& parameter, const InterfaceBuilder_ABC& builder )
: Param_ABC( parameter.GetName().c_str() )
, parameter_( parameter )
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
    // NOTHING
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
    stack_ = new QWidgetStack( parent );
    for( CIT_Params it = params_.begin(); it != params_.end(); ++it )
        widgets_.push_back( (*it)->BuildInterface( stack_ ) );
    return stack_;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::CheckValidity
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
bool ParamLocationComposite::CheckValidity()
{
    if( parameter_.IsOptional() )
        return true;
    if( selectedParam_ && selectedParam_->CheckValidity() )
        return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationComposite::CommitTo
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void ParamLocationComposite::CommitTo( actions::ParameterContainer_ABC& container ) const
{
    if( selectedParam_ )
        selectedParam_->CommitTo( container );
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
// Name: ParamLocationComposite::IsOptional
// Created: GGE 2011-08-02
// -----------------------------------------------------------------------------
bool ParamLocationComposite::IsOptional() const
{
    return parameter_.IsOptional();
}

