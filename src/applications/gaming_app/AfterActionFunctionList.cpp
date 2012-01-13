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
#include "actions/Parameter_ABC.h"
#include "actions_gui/ParamLocation.h"
#include "clients_kernel/Controller.h"
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
AfterActionFunctionList::AfterActionFunctionList( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory, AfterActionModel& model, actions::gui::InterfaceBuilder_ABC& interfaceBuilder )
    : Q3VBox( parent, "AfterActionFunctionList" )
    , builder_    ( interfaceBuilder )
    , controllers_( controllers )
    , model_      ( model )
    , parameters_ ( 0 )
    , request_    ( 0 )
    , title_      ( tr( "After action review" ) )
{
    functions_ = new ListDisplayer< AfterActionFunctionList >( this, *this, factory );
    functions_->AddColumn( tr( "Name" ) );
    functions_->viewport()->installEventFilter( new ListItemToolTip( functions_->viewport(), *functions_ ) );
    parameters_ = new Q3VGroupBox( tr( "Parameters" ), this );
    timeGroup_ = new Q3VGroupBox( tr( "Time range" ), this );
    timeGroup_->setCheckable( true );
    timeGroup_->setChecked( false );
    {
        Q3HBox* box = new Q3HBox( timeGroup_ );
        new QLabel( tr( "First tick" ), box );
        firstTick_ = new QSpinBox( box );
    }
    {
        Q3HBox* box = new Q3HBox( timeGroup_ );
        new QLabel( tr( "Duration" ), box );
        duration_ = new QSpinBox( box );
    }
    connect( functions_, SIGNAL( selectionChanged( Q3ListViewItem* ) ), SLOT( OnSelectionChange( Q3ListViewItem* ) ) );

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
void AfterActionFunctionList::OnSelectionChange( Q3ListViewItem* i )
{
    delete request_;
    request_ = 0;
    std::for_each( paramList_.begin(), paramList_.end(), boost::bind( &actions::gui::Param_ABC::RemoveFromController, _1 ) );
    paramList_.clear();
    delete parameters_;
    parameters_ = new Q3VGroupBox( tr( "Parameters" ), this );
    builder_.SetParamInterface( *this );
    builder_.SetParentObject( this );
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
    std::string compatibleType = "";
    unsigned int nbOccur = 1;
    if( type == "unit" )
        compatibleType = "agent";
    else if( type == "dotation list" )
        compatibleType = "allresourcetype";
    else if( type == "equipment list" )
        compatibleType = "maintenancepriorities";
    else if( type == "zone" )
        compatibleType = "location";
    else 
    {
        nbOccur = std::numeric_limits< int >::max();
        if( type == "unit list" )
            compatibleType = "agent";
        else if( type == "crowd list" )
            compatibleType = "crowd";
        else if( type == "urban block list" )
            compatibleType = "urbanknowledge";
        else if( type == "population list" )
            compatibleType = "inhabitant";
    }
    boost::shared_ptr< actions::gui::Param_ABC > result;
    if( !compatibleType.empty() )
    {
        const OrderParameter parameter( name.ascii(), compatibleType, false, 1, nbOccur );
        actions::gui::Param_ABC* param = &builder_.BuildOne( parameter, false );
        if( compatibleType == "location" )
            static_cast< actions::gui::ParamLocation* >( param )->SetShapeFilter( false, false, true, true, false );
        result.reset( param );
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

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::Title
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
QString AfterActionFunctionList::Title() const
{
    return title_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::GetIndex
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
int AfterActionFunctionList::GetIndex( actions::gui::Param_ABC* param ) const
{
    for( int i = 0; i < paramList_.size(); ++i )
        if( paramList_[ i ].get() == param )
            return i;
    return -1;
}
