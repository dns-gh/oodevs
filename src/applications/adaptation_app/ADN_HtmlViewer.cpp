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
#include <boost/filesystem.hpp>

// -----------------------------------------------------------------------------
// Name: ADN_HtmlViewer constructor
// Created: NPT 2013-01-04
// -----------------------------------------------------------------------------
ADN_HtmlViewer::ADN_HtmlViewer( QWidget* parent /*= 0*/ )
    : QWebView( parent )
{
    pConnector_ = new ADN_Connector_String< ADN_HtmlViewer >( this );
    assert(pConnector_);
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlViewer destructor
// Created: NPT 2013-01-04
// -----------------------------------------------------------------------------
ADN_HtmlViewer::~ADN_HtmlViewer()
{
    delete pConnector_;
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
    std::string val = text.toStdString();
    std::replace( val.begin(), val.end(), '\\', '/' );
    if( boost::filesystem::exists( val ) )
        load( QUrl( val.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlViewer::Warn
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
void ADN_HtmlViewer::Warn( ADN_ErrorStatus, const QString& )
{
    // NOTHING
}
