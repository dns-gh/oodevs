// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ToolListButton.h"
#include "moc_ToolListButton.cpp"

// -----------------------------------------------------------------------------
// Name: ToolListButtonBase constructor
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
ToolListButtonBase::ToolListButtonBase( const QIconSet& iconSet, const QString& toolTip, QToolBar* parent )
    : QToolButton( iconSet, "", "", 0, "", parent, "" )
    , toolTip_   ( toolTip )
    , menu_      ( 0 )
    , selected_  ( 0 )
{
    QToolTip::add( this, toolTip_ );
    setUsesTextLabel( true );                  
    setTextPosition( QToolButton::BesideIcon );
}

// -----------------------------------------------------------------------------
// Name: ToolListButtonBase::ToolListButtonBase
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
ToolListButtonBase::ToolListButtonBase( QToolBar* parent )
    : QToolButton( parent )
    , menu_      ( 0 )
    , selected_  ( 0 )
{
    setUsesTextLabel( true );                  
    setTextPosition( QToolButton::BesideIcon );
}

// -----------------------------------------------------------------------------
// Name: ToolListButtonBase destructor
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
ToolListButtonBase::~ToolListButtonBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToolListButtonBase::AddItem
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void ToolListButtonBase::AddItem( const QString& label, int i, bool selected /*= false*/ )
{
    if( ! menu_  )
    {
        menu_ = new QPopupMenu( this );
        setPopup( menu_ );
        setPopupDelay( 1 );
        connect( menu_, SIGNAL( activated( int ) ), this, SLOT( OnItemSelected( int ) ) );
        selected_ = 0;
    }
    int n = menu_->insertItem( label, i );
    if( selected )
    {
        menu_->setItemChecked( selected_, false );
        selected_ = n;
        menu_->setItemChecked( selected_, true );
        setTextLabel( label );
        OnSelected( i );
    }
    QToolTip::add( this, toolTip_ );
}

// -----------------------------------------------------------------------------
// Name: ToolListButtonBase::OnItemSelected
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void ToolListButtonBase::OnItemSelected( int id )
{
    Select( id );
    OnSelected( id );
}

// -----------------------------------------------------------------------------
// Name: ToolListButtonBase::Select
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void ToolListButtonBase::Select( int id )
{
    if( id == selected_ )
        return;

    menu_->setItemChecked( selected_, false );
    selected_ = id;
    menu_->setItemChecked( selected_, true );
    
    setTextLabel( menu_->text( selected_ ) );
    QToolTip::add( this, toolTip_ );
}
