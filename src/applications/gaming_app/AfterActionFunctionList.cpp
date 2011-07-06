// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionFunctionList.h"
#include "moc_AfterActionFunctionList.cpp"
#include "actions/ParameterContainer_ABC.h"
#include "actions_gui/ParamAgent.h"
#include "actions_gui/ParamAgentList.h"
#include "actions_gui/ParamCrowdList.h"
#include "actions_gui/ParamInhabitantList.h"
#include "actions_gui/ParamDotationTypeList.h"
#include "actions_gui/ParamEquipmentList.h"
#include "actions_gui/ParamUrbanBlockList.h"
#include "actions_gui/ParamLocation.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_gui/ListItemToolTip.h"
#include "clients_gui/ValuedListItem.h"
#include "gaming/AfterActionFunction.h"
#include "gaming/AfterActionModel.h"
#include "gaming/AfterActionParameter.h"
#include "gaming/IndicatorRequest.h"
#include "gaming/Simulation.h"
#include "gaming/StaticModel.h"
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList constructor
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
AfterActionFunctionList::AfterActionFunctionList( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory, AfterActionModel& model, ParametersLayer& layer, const ::StaticModel& staticModel )
    : QVBox( parent, "AfterActionFunctionList" )
    , controllers_( controllers )
    , model_      ( model )
    , layer_      ( layer )
    , staticModel_( staticModel )
    , parameters_ ( 0 )
    , request_    ( 0 )
{
    functions_ = new ListDisplayer< AfterActionFunctionList >( this, *this, factory );
    functions_->AddColumn( tr( "Name" ) );
    new ListItemToolTip( functions_->viewport(), *functions_ );
    parameters_ = new QVGroupBox( tr( "Parameters" ), this );
    timeGroup_ = new QVGroupBox( tr( "Time range" ), this );
    timeGroup_->setCheckable( true );
    timeGroup_->setChecked( false );
    {
        QHBox* box = new QHBox( timeGroup_ );
        new QLabel( tr( "First tick" ), box );
        firstTick_ = new QSpinBox( box );
    }
    {
        QHBox* box = new QHBox( timeGroup_ );
        new QLabel( tr( "Duration" ), box );
        duration_ = new QSpinBox( box );
    }
    connect( functions_, SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( OnSelectionChange( QListViewItem* ) ) );

    CreateRequestButton();
    request_->setEnabled( false );
    functions_->DeleteTail(
        functions_->DisplayList( model_.CreateIterator() )
        );
    controllers_.controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList destructor
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
AfterActionFunctionList::~AfterActionFunctionList()
{
    controllers_.controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::NotifyUpdated
// Created: JSR 2011-04-26
// -----------------------------------------------------------------------------
void AfterActionFunctionList::NotifyUpdated( const Simulation& simulation )
{
    firstTick_->setMinValue( 0 );
    firstTick_->setMaxValue( simulation.GetTickCount() );
    duration_->setMinValue( 0 );
    duration_->setMaxValue( simulation.GetTickCount() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::CreateRequestButton
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionFunctionList::CreateRequestButton()
{
    delete request_;
    request_ = new QPushButton( tr( "Request" ), this );
    QToolTip::add( request_, tr( "Send request" ) );
    connect( request_, SIGNAL( clicked() ), SLOT( Request() ) );
    request_->show();
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::Display
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
void AfterActionFunctionList::Display( const AfterActionFunction& function, Displayer_ABC&, ValuedListItem* item )
{
    item->SetNamed( function );
    item->SetToolTip( function.GetComments() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::OnSelectionChange
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
void AfterActionFunctionList::OnSelectionChange( QListViewItem* i )
{
    delete request_;
    request_ = 0;
    std::for_each( paramList_.begin(), paramList_.end(), boost::bind( &actions::gui::Param_ABC::RemoveFromController, _1 ) );
    paramList_.clear();
    delete parameters_;
    parameters_ = new QVGroupBox( tr( "Parameters" ), this );
    if( ValuedListItem* item = static_cast< ValuedListItem* >( i ) )
    {
        const AfterActionFunction* function = item->GetValue< const AfterActionFunction >();
        tools::Iterator< const AfterActionParameter& > it = function->CreateIterator();
        while( it.HasMoreElements() )
        {
            const AfterActionParameter& parameter = it.NextElement();
            CreateParameter( parameter );
        }
    }
    parameters_->show();
    CreateRequestButton();
    request_->setEnabled( i != 0 );
}

namespace
{
    struct Serializer : public actions::ParameterContainer_ABC
                      , private boost::noncopyable
    {
        Serializer( IndicatorRequest& request )
            : request_( &request )
        {
            // NOTHING
        }
        virtual void AddParameter( actions::Parameter_ABC& parameter )
        {
            std::string result;
            parameter.CommitTo( result );
            request_->SetParameter( parameter.GetName().ascii(), result );
            delete& parameter;
        }
    private:
        IndicatorRequest* request_;
    };
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::Request
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionFunctionList::Request()
{
    if( ValuedListItem* item = static_cast< ValuedListItem* >( functions_->selectedItem() ) )
        if( const AfterActionFunction* function = item->GetValue< const AfterActionFunction >() )
        {
            IndicatorRequest& request = model_.CreateRequest( *function );
            if( timeGroup_->isChecked() )
                request.SetTimeRange( firstTick_->value(), duration_->value() );
            Serializer serializer( request );
            std::for_each( paramList_.begin(), paramList_.end(),
                boost::bind( &actions::gui::Param_ABC::CommitTo, _1, boost::ref( serializer ) ) );
            request.Commit();
        }
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::CreateParameter
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< actions::gui::Param_ABC > AfterActionFunctionList::CreateParameter( const std::string& type, const QString& name )
{
    const OrderParameter parameter( name.ascii(), type.c_str(), false );
    const OrderParameter parameterList( name.ascii(), type.c_str(), false, 1, std::numeric_limits< int >::max() );

    boost::shared_ptr< actions::gui::Param_ABC > result;

    if( type == "unit" || type == "zone" )
    {
        const OrderParameter parameter( name.ascii(), type.c_str(), false );
        if( type == "unit" )
            result.reset( new actions::gui::ParamAgent( this, parameter, controllers_.controller_ ) );
        else if( type == "zone" )
        {
            std::auto_ptr< actions::gui::ParamLocation > location( new actions::gui::ParamLocation( parameter, layer_, staticModel_.coordinateConverter_ ) );
            location->SetShapeFilter( false, false, true, true, false );
            result.reset( location.release() );
        }
    }
    else
    {
        const OrderParameter parameter( name.ascii(), type.c_str(), false, 1, std::numeric_limits< int >::max() );
        if( type == "unit list" )
            result.reset( new actions::gui::ParamAgentList( this, OrderParameter( name.ascii(), type.c_str(), false, 1, std::numeric_limits< int >::max() ), controllers_.actions_, controllers_.controller_ ) );
        else if( type == "crowd list" )
            result.reset( new actions::gui::ParamCrowdList( this, parameter, controllers_.actions_, controllers_.controller_ ) );
        else if( type == "population list" )
            result.reset( new actions::gui::ParamInhabitantList( this, parameter, controllers_.actions_, controllers_.controller_ ) );
        else if( type == "dotation list" )
            result.reset( new actions::gui::ParamDotationTypeList( this, parameter, staticModel_.objectTypes_ ) );
        else if( type == "equipment list" )
            result.reset( new actions::gui::ParamEquipmentList( this, parameter, staticModel_.objectTypes_ ) );
        else if( type == "urban block list" )
            result.reset( new actions::gui::ParamUrbanBlockList( this, parameter, controllers_.actions_, controllers_.controller_ ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::CreateParameter
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void AfterActionFunctionList::CreateParameter( const AfterActionParameter& parameter )
{
    boost::shared_ptr< actions::gui::Param_ABC > pParameter = CreateParameter( parameter.GetType(), parameter.GetName() );
    if( pParameter )
    {
        paramList_.push_back( pParameter );
        pParameter->BuildInterface( parameters_ );
        pParameter->RegisterIn( controllers_.actions_ );
    }
    else
        new QLabel( parameter.GetName(), parameters_ );
}
