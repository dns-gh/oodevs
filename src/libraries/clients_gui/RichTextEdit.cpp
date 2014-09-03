// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTextEdit.h"
#include "moc_RichTextEdit.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichTextEdit constructor
// Created: ABR 2014-02-13
// -----------------------------------------------------------------------------
RichTextEdit::RichTextEdit( const QString& objectName, QWidget* parent /* = 0 */ )
    : RichWidget< QTextEdit >( objectName, parent )
    , cursorPos_( -1 )
{
    connect( this, SIGNAL( textChanged() ), this, SLOT( OnTextChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: RichTextEdit destructor
// Created: ABR 2014-02-13
// -----------------------------------------------------------------------------
RichTextEdit::~RichTextEdit()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichTextEdit::SaveCursor
// Created: ABR 2014-02-13
// -----------------------------------------------------------------------------
void RichTextEdit::SaveCursor()
{
    cursorPos_ = textCursor().position();
}

// -----------------------------------------------------------------------------
// Name: RichTextEdit::RestorCursor
// Created: ABR 2014-02-13
// -----------------------------------------------------------------------------
void RichTextEdit::RestoreCursor()
{
    if( cursorPos_ < 0 )
        return;
    QTextCursor cursor = textCursor();
    cursor.setPosition( cursorPos_, QTextCursor::MoveAnchor );
    setTextCursor( cursor );
    cursorPos_ = -1;
}

// -----------------------------------------------------------------------------
// Name: RichTextEdit::SetText
// Created: ABR 2014-02-13
// -----------------------------------------------------------------------------
void RichTextEdit::SetText( const QString& text )
{
    if( text == toPlainText() )
        return;
    SaveCursor();
    setPlainText( text );
    RestoreCursor();
}

// -----------------------------------------------------------------------------
// Name: RichTextEdit::SetBackgroundColor
// Created: ABR 2014-02-13
// -----------------------------------------------------------------------------
void RichTextEdit::SetBackgroundColor( const QColor& color )
{
    bool wasBlocked = blockSignals( true );
    QString str = toHtml();
    if( str.contains( "bgcolor=" ) )
        str.replace( QRegExp( "bgcolor=\"#[a-fA-F0-9]{6}\"" ), QString( "bgcolor=\"%1\"" ).arg( color.name() ) );
    else
        str.replace( "<body", QString( "<body bgcolor=\"%1\"" ).arg( color.name() ) );
    setHtml( str );
    blockSignals( wasBlocked );
}

// -----------------------------------------------------------------------------
// Name: RichTextEdit::OnTextChanged
// Created: ABR 2014-02-13
// -----------------------------------------------------------------------------
void RichTextEdit::OnTextChanged()
{
    if( !signalsBlocked() )
        emit TextChanged( toPlainText() );
}
