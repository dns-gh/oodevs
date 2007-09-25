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
#include "clients_gui/resources.h"
#include "gaming/AfterActionFunctions.h"
#include "gaming/AfterActionFunction.h"
#include "gaming/AfterActionParameter.h"
#include <qtoolbox.h>
#include <qvgroupbox.h>

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
{
    {
        QButtonGroup* buttons = new QButtonGroup( 2, Qt::Horizontal, this );
        buttons->layout()->setAlignment( Qt::AlignCenter );
        buttons->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
        buttons->setBackgroundMode( Qt::PaletteButton );
        buttons->setFrameStyle( QFrame::ToolBarPanel | QFrame::Raised );

        QToolButton* newBtn = new QToolButton( buttons );
        newBtn->setIconSet( MAKE_PIXMAP( new ) );
        QToolTip::add( newBtn, tr( "New function" ) );
        connect( newBtn, SIGNAL( clicked() ), SIGNAL( NewFunction() ) );

        QToolButton* editBtn = new QToolButton( buttons );
        editBtn->setIconSet( MAKE_PIXMAP( new ) ); // $$$$ AGE 2007-09-24: edit
        QToolTip::add( editBtn, tr( "Edit function" ) );
        connect( editBtn, SIGNAL( clicked() ), SLOT( EditFunction() ) );
    }

    functions_ = new ListDisplayer< AfterActionFunctionList >( this, *this, factory );
    functions_->AddColumn( tr( "Name" ) );
    parameters_ = new QVGroupBox( tr( "Parameters" ), this );

    connect( functions_, SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( OnSelectionChange( QListViewItem* ) ) );

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
    delete parameters_;
    parameters_ = new QVGroupBox( tr( "Parameters" ), this );
    if( ValuedListItem* item = static_cast< ValuedListItem* >( i ) )
    {
        const AfterActionFunction* function = item->GetValue< const AfterActionFunction >();
        Iterator< const AfterActionParameter& > it = function->CreateParameterIterator();
        while( it.HasMoreElements() )
        {
            const AfterActionParameter& parameter = it.NextElement();
            new QLabel( parameter.GetName(), parameters_ );
        }
    }
    parameters_->show();
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
