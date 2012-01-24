// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GUI_ABC.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 18:29 $
// $Revision: 3 $
// $Workfile: ADN_GUI_ABC.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GUI_ABC.h"
#include <QtGui/qsizepolicy.h>

// -----------------------------------------------------------------------------
// Name: ADN_GUI_ABC::tr
// Created: APE 2005-04-11
// -----------------------------------------------------------------------------
QString ADN_GUI_ABC::tr( const char* s, const char* c )
{
    if( qApp )
        return qApp->translate( strClassName_, s, c, QApplication::DefaultCodec );
    else
        return QString::fromLatin1( s );
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_ABC::CreateMainWidget
// Created: ABR 2012-01-20
// -----------------------------------------------------------------------------
QWidget* ADN_GUI_ABC::CreateScrollArea( QWidget& content, QWidget* list /*= 0*/, bool paintSplitter /*= true*/, bool paintBackground /*= false*/, bool showFrameBorder /*= true*/, int margin /*= 10*/, int spacing /*= 10*/ )
{
    // Content area
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget( &content );
    scrollArea->setWidgetResizable( true );

    // Backup color because splitter's background color applies to his children.
    QColor color = scrollArea->backgroundColor();

    // MainWidget
    QWidget* mainWidget = new QWidget( 0 );
    QHBoxLayout* pMainLayout = new QHBoxLayout( mainWidget );
    pMainLayout->setMargin( margin );
    pMainLayout->setSpacing( spacing );

    if( list == 0 )
        pMainLayout->addWidget( scrollArea, 1 );
    else
    {
        QSplitter* splitter = new QSplitter();
        pMainLayout->addWidget( splitter, 1 );
        splitter->setOpaqueResize( false );
        if( paintSplitter )
            splitter->setBackgroundColor( Qt::white );
        splitter->addWidget( list );
        splitter->addWidget( scrollArea );

        splitter->setCollapsible( list, false );
        splitter->setCollapsible( scrollArea, false );
        list->setMinimumWidth( 100 );
        list->setMaximumWidth( 350 );
    }

    scrollArea->setBackgroundColor( ( paintBackground ) ? Qt::white : color );
    if( !showFrameBorder )
        scrollArea->setFrameShape( QFrame::NoFrame );

    return mainWidget;
}
