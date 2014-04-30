// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SearchLineEdit.h"
#include "SearchButton.h"
#include "SubObjectName.h"
#include "ClearButton.h"

#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SearchLineEdit constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
SearchLineEdit::SearchLineEdit( const QString& objectName, QWidget* parent /* = 0*/ )
    : RichLineEdit( objectName, parent )
{
    SubObjectName subObject( objectName );
    // search icon on the left
    AddWidget( new SearchButton( "SearchButton", this ), RichLineEdit::LeftSide );

    // clear button on the right
    ClearButton* clearButton = new ClearButton( this );
    AddWidget( clearButton, RichLineEdit::RightSide );
    clearButton->hide();

    // Connection
    connect( clearButton, SIGNAL( clicked() ), this, SLOT( clear() ) );
    connect( this, SIGNAL( textChanged( const QString& ) ), clearButton, SLOT( TextChanged( const QString& ) ) );

    // Initialization
    UpdateTextMargins();
    SetInactiveText( tools::translate( "gui::SearchLineEdit", "Search") );
}

// -----------------------------------------------------------------------------
// Name: SearchLineEdit destructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
SearchLineEdit::~SearchLineEdit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SearchLineEdit::Apply
// Created: ABR 2014-04-28
// -----------------------------------------------------------------------------
bool SearchLineEdit::Apply( QStandardItem& item ) const
{
    auto filter = text().lower();
    //if( filter .size() > 0 && filter [ 0 ] == '<' && filter [ filterLine.size() - 1 ] == '>' )
    //    return;
    if( filter.isEmpty() )
        return true;
    QStringList list = filter.split( ' ' );
    bool result = true;
    QString text = item.text().lower();
    for( QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it )
        result = result && text.find( *it ) != -1;
    return result;
}

// -----------------------------------------------------------------------------
// Name: SearchLineEdit::Clear
// Created: ABR 2014-04-28
// -----------------------------------------------------------------------------
void SearchLineEdit::Clear()
{
    clear();
}
