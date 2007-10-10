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
#include "clients_kernel/Controllers.h"
#include "clients_gui/ValuedListItem.h"
#include "gaming/AfterActionFunction.h"
#include "gaming/AfterActionParameter.h"
#include "gaming/AfterActionRequest.h"
#include "gaming/AfterActionModel.h"
#include "gaming/ActionParameterContainer_ABC.h"
#include "ParamAgent.h"
#include "ParamAgentList.h"
#include "icons.h"
#include <qtoolbox.h>
#include <qvgroupbox.h>
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList constructor
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
AfterActionFunctionList::AfterActionFunctionList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, AfterActionModel& model )
    : QVBox( parent )
    , controllers_( controllers )
    , model_( model )
    , parameters_( 0 )
    , request_( 0 )
{
    functions_ = new ListDisplayer< AfterActionFunctionList >( this, *this, factory );
    functions_->AddColumn( tr( "Name" ) );
    parameters_ = new QVGroupBox( tr( "Parameters" ), this );
    connect( functions_, SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( OnSelectionChange( QListViewItem* ) ) );

    CreateRequestButton();
    request_->setEnabled( false );
    functions_->DeleteTail(
        functions_->DisplayList( model_.CreateIterator() )
        );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList destructor
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
AfterActionFunctionList::~AfterActionFunctionList()
{
    // NOTHING
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
void AfterActionFunctionList::Display( const AfterActionFunction& function, Displayer_ABC& , ValuedListItem* item )
{
    item->SetNamed( function );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::OnSelectionChange
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
void AfterActionFunctionList::OnSelectionChange( QListViewItem* i )
{
    delete request_; request_ = 0;
    std::for_each( paramList_.begin(), paramList_.end(), boost::bind( &Param_ABC::RemoveFromController, _1 ) );
    paramList_.clear();
    delete parameters_;
    parameters_ = new QVGroupBox( tr( "Parameters" ), this );
    if( ValuedListItem* item = static_cast< ValuedListItem* >( i ) )
    {
        const AfterActionFunction* function = item->GetValue< const AfterActionFunction >();
        Iterator< const AfterActionParameter& > it = function->CreateIterator();
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
    struct Serializer : public ActionParameterContainer_ABC
    {
        Serializer( AfterActionRequest& request )
            : request_( &request ) {}
        virtual void AddParameter( ActionParameter_ABC& parameter )
        {
            std::string result;
            parameter.CommitTo( result );
            request_->SetParameter( parameter.GetName().ascii(), result );
            delete& parameter;
        }
    private:
        Serializer( const Serializer& );
        Serializer& operator=( const Serializer& );
        AfterActionRequest* request_;
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
            AfterActionRequest& request = model_.CreateRequest( *function );
            Serializer serializer( request );
            std::for_each( paramList_.begin(), paramList_.end(),
                boost::bind( &Param_ABC::CommitTo, _1, boost::ref( serializer ) ) );
            request.Commit();
        }
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::CreateParameter
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< Param_ABC > AfterActionFunctionList::CreateParameter( const std::string& type, const QString& name )
{
    boost::shared_ptr< Param_ABC > result;
    if( type == "unit" )
        result.reset( new ParamAgent( this, kernel::OrderParameter( name, type.c_str(), false ) ) );
    else if( type == "unit list" )
        result.reset( new ParamAgentList( this, kernel::OrderParameter( name, type.c_str(), false ), controllers_.actions_ ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::CreateParameter
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void AfterActionFunctionList::CreateParameter( const AfterActionParameter& parameter )
{
    boost::shared_ptr< Param_ABC > pParameter = CreateParameter( parameter.GetType(), parameter.GetName() );
    if( pParameter )
    {
        paramList_.push_back( pParameter );
        pParameter->BuildInterface( parameters_ );
        pParameter->RegisterIn( controllers_.actions_ );
    }
    else
        new QLabel( parameter.GetName(), parameters_ );
}
