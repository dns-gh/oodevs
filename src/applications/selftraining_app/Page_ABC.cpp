// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Page_ABC.h"
#include "moc_Page_ABC.cpp"
#include "MenuButton.h"
#include "QuitPage.h"
#include "clients_gui/Tools.h"

QuitPage* Page_ABC::quitPage_ = 0;

// -----------------------------------------------------------------------------
// Name: Page_ABC constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Page_ABC::Page_ABC( Q3WidgetStack* pages, Page_ABC& previous, unsigned short flags )
    : Q3VBox ( pages )
    , pages_( pages )
    , previous_( previous )
    , startButton_( 0 )
    , joinButton_( 0 )
    , editButton_( 0 )
{
    setBackgroundOrigin( QWidget::WindowOrigin );

    //layout()->setAlignment( Qt::AlignTop );
    grid_ = new Q3GridLayout( layout(), 3, 2 );
    grid_->setRowStretch( 0, 1 );
    grid_->setRowStretch( 1, 10 );
    grid_->setRowStretch( 2, 2 );

    Q3HBoxLayout* buttonLayout = new Q3HBoxLayout();
    buttonLayout->setMargin( 20 );
    grid_->addMultiCellLayout( buttonLayout, 2, 2, 0, 2 );

    if( flags & eButtonBack )
    {
        QPushButton* button = new MenuButton( tools::translate( "Page_ABC", "Back" ), this );
        button->setFixedWidth( 150 );
        buttonLayout->addWidget( button, 0, Qt::AlignBottom | Qt::AlignLeft );
        connect( button, SIGNAL( clicked() ), this, SLOT( OnBack() ) );
    }
    else if( flags & eButtonOptions )
    {
        QPushButton* button = new MenuButton( tools::translate( "Page_ABC", "Options" ), this );
        button->setFixedWidth( 150 );
        buttonLayout->addWidget( button, 0, Qt::AlignBottom | Qt::AlignLeft );
        connect( button, SIGNAL( clicked() ), this, SLOT( OnOptions() ) );
    }

    if( flags & eButtonQuit )
    {
        if( !quitPage_ )
            quitPage_ = new QuitPage( pages, *this );

        QPushButton* button = new MenuButton( tools::translate( "Page_ABC", "Quit" ), this );
        button->setFixedWidth( 150 );
        buttonLayout->addWidget( button, 0, Qt::AlignBottom | Qt::AlignRight );
        connect( button, SIGNAL( clicked() ), this, SLOT( OnQuit() ) );
    }

    if( flags & eButtonStart )
    {
        startButton_ = new MenuButton( tools::translate( "Page_ABC", "Start" ), this );
        startButton_->setFixedWidth( 150 );
        buttonLayout->addWidget( startButton_, 0, Qt::AlignBottom | Qt::AlignRight );
        connect( startButton_, SIGNAL( clicked() ), this, SLOT( OnStart() ) );
    }
    else if( flags & eButtonJoin )
    {
        joinButton_ = new MenuButton( tools::translate( "Page_ABC", "Join" ), this );
        joinButton_->setFixedWidth( 150 );
        buttonLayout->addWidget( joinButton_, 0, Qt::AlignBottom | Qt::AlignRight );
        connect( joinButton_, SIGNAL( clicked() ), this, SLOT( OnJoin() ) );
    }
    else if( flags & eButtonEdit )
    {
        editButton_ = new MenuButton( tools::translate( "Page_ABC", "Edit" ), this );
        editButton_->setFixedWidth( 150 );
        buttonLayout->addWidget( editButton_, 0, Qt::AlignBottom | Qt::AlignRight );
        connect( editButton_, SIGNAL( clicked() ), this, SLOT( OnEdit() ) );
    }

    layout()->setAutoAdd( false );
    hide();
}

// -----------------------------------------------------------------------------
// Name: Page_ABC destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Page_ABC::~Page_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::showEvent
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void Page_ABC::showEvent( QShowEvent* event )
{
    pages_->raiseWidget( this );
    Update();
    Q3VBox::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::AddContent
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void Page_ABC::AddContent( QWidget* widget )
{
    grid_->addMultiCellWidget( widget, 1, 1, 0, 2 );
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::AddTitle
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void Page_ABC::AddTitle( const QString& title )
{
    QLabel* label = new QLabel( title, this );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    QFont font( font() );
    font.setPixelSize( 30 );
    font.setItalic( true );
    label->setFont( font );
    label->setFixedHeight( 50 );
    grid_->addMultiCellWidget( label, 0, 0, 0, 2, Qt::AlignVCenter | Qt::AlignLeft );
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::EnableButton
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void Page_ABC::EnableButton( unsigned short flags, bool enable )
{
    if( ( flags & eButtonStart ) && startButton_ )
        startButton_->setEnabled( enable );
    if( ( flags & eButtonJoin ) && joinButton_ )
        joinButton_->setEnabled( enable );
    if( ( flags & eButtonEdit ) && editButton_ )
        editButton_->setEnabled( enable );
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::SetButtonText
// Created: JSR 2010-07-19
// -----------------------------------------------------------------------------
void Page_ABC::SetButtonText( unsigned short flags, const QString& text )
{
    if( ( flags & eButtonStart ) && startButton_ )
        startButton_->setText( text );
    if( ( flags & eButtonJoin ) && joinButton_ )
        joinButton_->setText( text );
    if( ( flags & eButtonEdit ) && editButton_ )
        editButton_->setText( text );
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::Previous
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
void Page_ABC::Previous()
{
    qApp->postEvent( &previous_, new QShowEvent() );
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::OnBack
// Created: JSR 2010-06-10
// -----------------------------------------------------------------------------
void Page_ABC::OnBack()
{
    previous_.show();
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::OnQuit
// Created: JSR 2010-06-10
// -----------------------------------------------------------------------------
void Page_ABC::OnQuit()
{
    quitPage_->show();
}
