// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_TextEdit_ABC.h"
#include "moc_ADN_TextEdit_ABC.cpp"
#include "ADN_Ref_ABC.h"

//-----------------------------------------------------------------------------
// Name: ADN_TextEdit_ABC constructor
// Created: JSR 21-11-11
//-----------------------------------------------------------------------------
ADN_TextEdit_ABC::ADN_TextEdit_ABC( QWidget* parent /* = 0 */, const char * name)
    : QTextEdit( parent,name )
    , ADN_Gfx_ABC()
    , originalPalette_( palette() )
    , originalToolTip_( "" )
{
    // connect edit line & connector
    connect( this, SIGNAL( textChanged() ), this, SLOT( TextChanged() ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_TextEdit_ABC destructor
// Created: JSR 21-11-11
//-----------------------------------------------------------------------------
ADN_TextEdit_ABC::~ADN_TextEdit_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_TextEdit_ABC::SetToolTip
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_TextEdit_ABC::SetToolTip( const QString& toolTip )
{
    originalToolTip_ = toolTip;
    setToolTip( originalToolTip_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_TextEdit_ABC::ConnectWithRefValidity
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_TextEdit_ABC::ConnectWithRefValidity( const ADN_Ref_ABC& ref )
{
    connect( this, SIGNAL( textChanged( const QString& ) ), &ref, SLOT( CheckValidity() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_TextEdit_ABC::ChangeBackgroundColor
// Created: ABR 2013-09-20
// -----------------------------------------------------------------------------
void ADN_TextEdit_ABC::ChangeBackgroundColor( const QColor& color )
{
    QString str = toHtml();
    if( str.contains( "bgcolor=" ) )
        str.replace( QRegExp( "bgcolor=\"#[a-fA-F0-9]{6}\"" ), QString( "bgcolor=\"%1\"" ).arg( color.name() ) );
    else
        str.replace( "<body", QString( "<body bgcolor=\"%1\"" ).arg( color.name() ) );
    setHtml( str );
}

// -----------------------------------------------------------------------------
// Name: ADN_TextEdit_ABC::Warn
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
void ADN_TextEdit_ABC::Warn( ADN_ErrorStatus errorStatus, const QString& errorMessage /*= ""*/ )
{
    switch( errorStatus )
    {
    case eNoError:
        {
            setToolTip( originalToolTip_ );
            ChangeBackgroundColor( Qt::white );
            setPalette( originalPalette_ );
        }
        break;
    case eWarning:
        {
            if( !errorMessage.isEmpty() )
                setToolTip( errorMessage );
            ChangeBackgroundColor( Qt::yellow );
            QPalette palette;
            palette.setColor( QPalette::Text, Qt::black );
            palette.setColor( QPalette::Base, Qt::yellow );
            palette.setColor( QPalette::Shadow, Qt::yellow );
            palette.setColor( QPalette::Window, Qt::yellow );
            palette.setColor( QPalette::Highlight, QColor( Qt::yellow ).lighter( 50 ) );
            palette.setColor( QPalette::HighlightedText, Qt::black );
            setPalette( palette );
        }
        break;
    case eError:
        {
            if( !errorMessage.isEmpty() )
                setToolTip( errorMessage );
            ChangeBackgroundColor( Qt::red );
            QPalette palette;
            palette.setColor( QPalette::Text, Qt::black );
            palette.setColor( QPalette::Base, Qt::red );
            palette.setColor( QPalette::Shadow, Qt::red );
            palette.setColor( QPalette::Window, Qt::red );
            palette.setColor( QPalette::Highlight, QColor( Qt::red ).lighter( 150 ) );
            palette.setColor( QPalette::HighlightedText, Qt::black );
            setPalette( palette );
        }
        break;
    default:
        break;
    }
}
