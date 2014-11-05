// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichPathWidget.h"
#include "moc_RichPathWidget.cpp"
#include "FileDialog.h"
#include "RichLineEdit.h"
#include "RichPushButton.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichPathWidget constructor
// Created: JSR 2014-10-29
// -----------------------------------------------------------------------------
RichPathWidget::RichPathWidget( const QString& objectName, const QString& caption, const tools::Path& dir, const QString& filter, QWidget* parent /*= 0*/ )
    : RichWidget< QWidget >( objectName, parent )
    , caption_( caption )
    , filter_( filter )
    , dir_( dir )
{
    edit_ = new RichLineEdit( objectName + "-edit" );
    RichPushButton* button = new RichPushButton( objectName + "-browse", tr( "Browse..." ) );

    QHBoxLayout* layout = new QHBoxLayout;
    setLayout( layout );

    layout->addWidget( edit_ );
    layout->addWidget( button );

    connect( edit_, SIGNAL( textChanged( const QString& ) ), SLOT( TextChanged( const QString& ) ) );
    connect( button, SIGNAL( clicked() ), SLOT( ButtonClicked() ) );
}

// -----------------------------------------------------------------------------
// Name: RichPathWidget destructor
// Created: JSR 2014-10-29
// -----------------------------------------------------------------------------
RichPathWidget::~RichPathWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichPathWidget::SetPath
// Created: JSR 2014-10-29
// -----------------------------------------------------------------------------
void RichPathWidget::SetPath( const tools::Path& path )
{
    edit_->SetText( QString::fromStdString( path.ToUTF8() ) );
}

// -----------------------------------------------------------------------------
// Name: RichPathWidget::GetPath
// Created: JSR 2014-10-29
// -----------------------------------------------------------------------------
tools::Path RichPathWidget::GetPath() const
{
    return tools::Path::FromUTF8( edit_->text().toStdString() );
}

// -----------------------------------------------------------------------------
// Name: RichPathWidget::TextChanged
// Created: JSR 2014-10-29
// -----------------------------------------------------------------------------
void RichPathWidget::TextChanged( const QString& string )
{
    emit PathChanged( tools::Path::FromUTF8( string.toStdString() ) );
}

// -----------------------------------------------------------------------------
// Name: RichPathWidget::ButtonClicked
// Created: JSR 2014-10-29
// -----------------------------------------------------------------------------
void RichPathWidget::ButtonClicked()
{
    tools::Path filename = FileDialog::getOpenFileName( this, caption_, dir_, filter_ );
    if( filename.IsEmpty() )
        return;
    filename.MakePreferred();
    SetPath( filename );
}
