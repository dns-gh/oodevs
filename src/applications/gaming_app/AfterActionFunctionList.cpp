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
#include "gaming/AfterActionFunctions.h"
#include "gaming/AfterActionFunction.h"
#include "gaming/AfterActionParameter.h"
#include "gaming/ActionParameterContainer_ABC.h"
#include "ParamAgent.h"
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
AfterActionFunctionList::AfterActionFunctionList( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory )
    : QVBox( parent )
    , controllers_( controllers )
    , parameters_( 0 )
    , request_( 0 )
{
    {
        QButtonGroup* buttons = new QButtonGroup( 2, Qt::Horizontal, this );
        buttons->layout()->setAlignment( Qt::AlignCenter );
        buttons->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
        buttons->setBackgroundMode( Qt::PaletteButton );
        buttons->setFrameStyle( QFrame::ToolBarPanel | QFrame::Raised );

        QToolButton* newBtn = new QToolButton( buttons );
        newBtn->setIconSet( MAKE_PIXMAP( aaa_function_new ) );
        QToolTip::add( newBtn, tr( "New function" ) );
        connect( newBtn, SIGNAL( clicked() ), SIGNAL( NewFunction() ) );

        QToolButton* editBtn = new QToolButton( buttons );
        editBtn->setIconSet( MAKE_PIXMAP( aaa_function_edit ) );
        QToolTip::add( editBtn, tr( "Edit function" ) );
        connect( editBtn, SIGNAL( clicked() ), SLOT( EditFunction() ) );
    }

    functions_ = new ListDisplayer< AfterActionFunctionList >( this, *this, factory );
    functions_->AddColumn( tr( "Name" ) );
    parameters_ = new QVGroupBox( tr( "Parameters" ), this );
    connect( functions_, SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( OnSelectionChange( QListViewItem* ) ) );

    CreateRequestButton();
    request_->setEnabled( false );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList destructor
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
AfterActionFunctionList::~AfterActionFunctionList()
{
    controllers_.Unregister( *this );
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
// Name: AfterActionFunctionList::NotifyUpdated
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
void AfterActionFunctionList::NotifyUpdated( const AfterActionFunctions& model )
{
    functions_->DeleteTail(
        functions_->DisplayList( model.CreateIterator() )
        );
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
    delete parameters_;
    std::for_each( paramList_.begin(), paramList_.end(), boost::bind( &Param_ABC::RemoveFromController, _1 ) );
    paramList_.clear();
    parameters_ = new QVGroupBox( tr( "Parameters" ), this );
    if( ValuedListItem* item = static_cast< ValuedListItem* >( i ) )
    {
        const AfterActionFunction* function = item->GetValue< const AfterActionFunction >();
        Iterator< const AfterActionParameter& > it = function->CreateParameterIterator();
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

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::EditFunction
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
void AfterActionFunctionList::EditFunction()
{
    if( ValuedListItem* item = static_cast< ValuedListItem* >( functions_->selectedItem() ) )
        emit EditFunction( item->GetValue< const AfterActionFunction >() );
}

namespace
{
    struct Serializer : public ActionParameterContainer_ABC
    {
        Serializer( const AfterActionFunction& function )
            : function_( const_cast< AfterActionFunction* >( &function ) ) {}
        virtual void AddParameter( ActionParameter_ABC& parameter )
        {
            std::string result;
            parameter.CommitTo( result );
            function_->SetParameter( parameter.GetName(), result );
            delete& parameter;
        }
    private:
        Serializer( const Serializer& );
        Serializer& operator=( const Serializer& );
        AfterActionFunction* function_;
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
            Serializer serializer( *function );
            std::for_each( paramList_.begin(), paramList_.end(), 
                boost::bind( &Param_ABC::CommitTo, _1, boost::ref( serializer ) ) );
            emit CreateRequest( function );
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
