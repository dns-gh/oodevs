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

namespace
{
    QPushButton* AddButton( QWidget* parent, Q3HBoxLayout* buttonLayout, Qt::Alignment alignment, const char* slot, int width = 160 )
    {
        QPushButton* button =  new MenuButton( parent );
        button->setFixedWidth( width );
        buttonLayout->addWidget( button, 0, alignment );
        QObject::connect( button, SIGNAL( clicked() ), parent, slot );
        return button;
    }
}

// -----------------------------------------------------------------------------
// Name: Page_ABC constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Page_ABC::Page_ABC( Q3WidgetStack* pages, Page_ABC& previous, unsigned short flags )
    : gui::LanguageChangeObserver_ABC< Q3VBox >( pages )
    , pages_         ( pages )
    , previous_      ( previous )
    , backButton_    ( 0 )
    , settingsButton_( 0 )
    , quitButton_    ( 0 )
    , startButton_   ( 0 )
    , joinButton_    ( 0 )
    , editButton_    ( 0 )
    , applyButton_   ( 0 )
    , titleLabel_    ( 0 )
{
    grid_ = new Q3GridLayout( layout(), 3, 2 );
    grid_->setRowStretch( 0, 1 );
    grid_->setRowStretch( 1, 10 );
    grid_->setRowStretch( 2, 2 );

    Q3HBoxLayout* buttonLayout = new Q3HBoxLayout();
    buttonLayout->setMargin( 20 );
    grid_->addMultiCellLayout( buttonLayout, 2, 2, 0, 2 );

    if( flags & eButtonBack )
        backButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignLeft, SLOT( OnBack() ) );
    else if( flags & eButtonOptions )
        settingsButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignLeft, SLOT( OnOptions() ) );

    if( flags & eButtonQuit )
    {
        if( !quitPage_ )
            quitPage_ = new QuitPage( pages, *this );
        quitButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnQuit() ) );
    }

    if( flags & eButtonStart )
        startButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnStart() ) );
    else if( flags & eButtonJoin )
        joinButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnJoin() ) );
    else if( flags & eButtonEdit )
        editButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnEdit() ) );
    else if( flags & eButtonApply )
        applyButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnApply() ) );

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
// Name: Page_ABC::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void Page_ABC::OnLanguageChanged()
{
    if( backButton_ )
        backButton_->setText(     tools::translate( "Page_ABC", "Back" ) );
    if( settingsButton_ )
        settingsButton_->setText( tools::translate( "Page_ABC", "Settings" ) );
    if( quitButton_ )
        quitButton_->setText(     tools::translate( "Page_ABC", "Quit" ) );
    if( startButton_ )
        startButton_->setText(    tools::translate( "Page_ABC", "Start" ) );
    if( joinButton_ )
        joinButton_->setText(     tools::translate( "Page_ABC", "Join" ) );
    if( editButton_ )
        editButton_->setText(     tools::translate( "Page_ABC", "Edit" ) );
    if( applyButton_ )
        applyButton_->setText(    tools::translate( "Page_ABC", "Apply" ) );
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
// Name: Page_ABC::SetTitle
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void Page_ABC::SetTitle( const QString& title )
{
    if( !titleLabel_ )
    {
        titleLabel_ = new QLabel( this );
        QFont font( font() );
        font.setPixelSize( 30 );
        font.setItalic( true );
        titleLabel_->setFont( font );
        titleLabel_->setFixedHeight( 50 );
        grid_->addMultiCellWidget( titleLabel_, 0, 0, 0, 2, Qt::AlignVCenter | Qt::AlignLeft );
    }
    titleLabel_->setText( title );
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::GetTitle
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
const QString Page_ABC::GetTitle() const
{
    if( titleLabel_ )
        return titleLabel_->text();
    return "";
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
    if( ( flags & eButtonApply ) && applyButton_ )
        applyButton_->setEnabled( enable );
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
