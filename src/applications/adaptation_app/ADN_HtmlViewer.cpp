// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_HtmlViewer.h"
#include "moc_ADN_HtmlViewer.cpp"
#include "ADN_Connector_String.h"

// -----------------------------------------------------------------------------
// Name: ADN_HtmlViewer constructor
// Created: NPT 2013-01-04
// -----------------------------------------------------------------------------
ADN_HtmlViewer::ADN_HtmlViewer( QWidget* parent /* = 0 */ )
    : QWebView( parent )
{
    pConnector_.reset( new ADN_Connector_String< ADN_HtmlViewer >( this ) );
    setContextMenuPolicy( Qt::CustomContextMenu );
    connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenu( const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlViewer destructor
// Created: NPT 2013-01-04
// -----------------------------------------------------------------------------
ADN_HtmlViewer::~ADN_HtmlViewer()
{
    // NOTHING
}
 
// -----------------------------------------------------------------------------
// Name: ADN_HtmlViewer::text
// Created: NPT 2013-01-04
// -----------------------------------------------------------------------------
QString ADN_HtmlViewer::text() const
{
    return url().isEmpty() ? "" : url().path();
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlViewer::setText
// Created: NPT 2013-01-04
// -----------------------------------------------------------------------------
void ADN_HtmlViewer::setText( const QString& text )
{
    setHtml( "<html/>" );
    tools::Path val = tools::Path::FromUnicode( text.toStdWString() );
    if( val.Exists() )
        load( QUrl::fromLocalFile( QString::fromStdWString( val.Normalize().ToUnicode() ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlViewer::Warn
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
void ADN_HtmlViewer::Warn( ADN_ErrorStatus, const QString& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlViewer::OnContextMenu
// Created: NPT 2013-04-30
// -----------------------------------------------------------------------------
void ADN_HtmlViewer::OnContextMenu( const QPoint& pos )
{
    QMenu menu;
    menu.addAction( pageAction( QWebPage::Copy ) );
    menu.exec( mapToGlobal( pos ) );
}
