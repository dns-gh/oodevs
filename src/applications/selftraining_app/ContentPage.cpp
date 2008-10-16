// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ContentPage.h"
#include "MenuButton.h"
#include "clients_gui/Tools.h"
#include <qlayout.h>
#include <qfont.h>
#include <qlabel.h>

// -----------------------------------------------------------------------------
// Name: ContentPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ContentPage::ContentPage( QWidgetStack* pages, const QString& title, Page_ABC& previous )
    : Page_ABC( pages )
    , previous_ ( previous ) 
{
    setMargin( 20 );
    layout()->setAlignment( Qt::AlignTop );
    grid_ = new QGridLayout( layout(), 3, 2 );
    grid_->setRowStretch( 0, 1 ); 
    grid_->setRowStretch( 1, 10 );
    grid_->setRowStretch( 2, 1 );
    AddTitle( title );
    AddBackButton( previous );
    layout()->setAutoAdd( false );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ContentPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ContentPage::~ContentPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContentPage::AddTitle
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ContentPage::AddTitle( const QString& title )
{
    QLabel* label = new QLabel( title, this );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    QFont font( font() );
    font.setPixelSize( 30 );
    font.setItalic( true );
    label->setFont( font );
    label->setFixedHeight( 50 );
    grid_->addMultiCellWidget( label, 0, 0, 0, 1, Qt::AlignVCenter | Qt::AlignLeft );
}

// -----------------------------------------------------------------------------
// Name: ContentPage::AddBackButton
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ContentPage::AddBackButton( Page_ABC& previous )
{
    QButton* button = new MenuButton( tools::translate( "ContentPage", "Back" ), this );
//    button->setFixedSize( 100, 40 );
    grid_->addWidget( button, 2, 0, Qt::AlignBottom | Qt::AlignLeft );
    connect( button, SIGNAL( clicked() ), &previous, SLOT( show() ) );
}

// -----------------------------------------------------------------------------
// Name: ContentPage::AddContent
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ContentPage::AddContent( QWidget* widget )
{
    grid_->addMultiCellWidget( widget, 1, 1, 0, 1 );
}

// -----------------------------------------------------------------------------
// Name: ContentPage::AddNextButton
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ContentPage::AddNextButton( const QString& caption, Page_ABC& page, const char* slot /*= 0*/ )
{
    QButton* button = AddNextButton( caption ) ; 
    connect( button, SIGNAL( clicked() ), &page, slot ? slot : SLOT( show() ) );
}

// -----------------------------------------------------------------------------
// Name: ContentPage::AddNextButton
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
QButton* ContentPage::AddNextButton( const QString& caption )
{
    QButton* button = new MenuButton( caption, this );
    grid_->addWidget( button, 2, 1, Qt::AlignBottom | Qt::AlignRight );
    return button; 
}

// -----------------------------------------------------------------------------
// Name: ContentPage::Previous
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
void ContentPage::Previous()
{
    qApp->postEvent( &previous_, new QShowEvent() );
}
