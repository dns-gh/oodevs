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
#include "clients_kernel/Tools.h"

QuitPage* Page_ABC::quitPage_ = 0;

namespace
{
    QPushButton* AddButton( QWidget* parent, QHBoxLayout* buttonLayout, Qt::Alignment alignment, const char* slot, int width = 160 )
    {
        QPushButton* button =  new MenuButton();
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
Page_ABC::Page_ABC( QStackedWidget* pages, Page_ABC& previous, unsigned short flags )
    : gui::WidgetLanguageObserver_ABC< QWidget >( pages )
    , pages_        ( pages )
    , previous_     ( previous )
    , backButton_   ( 0 )
    , adminButton_  ( 0 )
    , quitButton_   ( 0 )
    , startButton_  ( 0 )
    , joinButton_   ( 0 )
    , editButton_   ( 0 )
    , applyButton_  ( 0 )
    , deleteButton_ ( 0 )
    , upgradeButton_( 0 )
    , cancelButton_ ( 0 )
    , exportButton_ ( 0 )
    , titleLabel_   ( 0 )
{
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setContentsMargins( 0, 0, 0, 0 ); 
    grid_ = new QGridLayout();
    layout->addLayout( grid_ );
    grid_->setMargin( 0 );
    grid_->setRowStretch( 0, 1 );
    grid_->setRowStretch( 1, 10 );
    grid_->setRowStretch( 2, 2 );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin( 20 );
    grid_->addLayout( buttonLayout, 2, 0, 1, 2 );

    if( flags & eButtonBack )
        backButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignLeft, SLOT( OnBack() ) );
    else if( flags & eButtonAdmin )
        adminButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignLeft, SLOT( OnOptions() ) );

    if( flags & eButtonQuit )
    {
        if( !quitPage_ )
            quitPage_ = new QuitPage( pages, *this );
        quitButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnQuit() ) );
    }

    if( flags & eButtonDelete )
        deleteButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnDelete() ) );
    if( flags & eButtonUpgrade )
        upgradeButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnUpgrade() ) );
    if( flags & eButtonCancel )
        cancelButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnCancel() ) );
    if( flags & eButtonExport )
        exportButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnExport() ) );
    if( flags & eButtonStart )
        startButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnStart() ) );
    else if( flags & eButtonJoin )
        joinButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnJoin() ) );
    else if( flags & eButtonEdit )
        editButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnEdit() ) );
    else if( flags & eButtonApply )
        applyButton_ = AddButton( this, buttonLayout, Qt::AlignBottom | Qt::AlignRight, SLOT( OnApply() ) );

    pages_->addWidget( this );
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
        backButton_->setText(   tools::translate( "Page_ABC", "Back" ) );
    if( adminButton_ )
        adminButton_->setText(  tools::translate( "Page_ABC", "Admin" ) );
    if( quitButton_ )
        quitButton_->setText(   tools::translate( "Page_ABC", "Quit" ) );
    if( startButton_ )
        startButton_->setText(  tools::translate( "Page_ABC", "Start" ) );
    if( joinButton_ )
        joinButton_->setText(   tools::translate( "Page_ABC", "Join" ) );
    if( editButton_ )
        editButton_->setText(   tools::translate( "Page_ABC", "Edit" ) );
    if( applyButton_ )
        applyButton_->setText(  tools::translate( "Page_ABC", "Apply" ) );
    if( deleteButton_ )
        deleteButton_->setText( tools::translate( "Page_ABC", "Delete" ) );
    if( upgradeButton_ )
        upgradeButton_->setText( tools::translate( "Page_ABC", "Upgrade" ) );
    if( cancelButton_ )
        cancelButton_->setText( tools::translate( "Page_ABC", "Cancel" ) );
    if( exportButton_ )
        exportButton_->setText( tools::translate( "Page_ABC", "Export" ) );
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::showEvent
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void Page_ABC::showEvent( QShowEvent* event )
{
    if( pages_->indexOf( this ) == 0 )
        pages_->addWidget( this );
    pages_->setCurrentWidget( this );
    Update();
    QWidget::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::AddContent
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void Page_ABC::AddContent( QWidget* widget )
{
    grid_->addWidget( widget, 1, 0, 1, 2 );
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
        grid_->addWidget( titleLabel_, 0, 0, 1, 2, Qt::AlignVCenter | Qt::AlignLeft );
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
    if( ( flags & eButtonBack ) && backButton_ )
        backButton_->setEnabled( enable );
    if( ( flags & eButtonStart ) && startButton_ )
        startButton_->setEnabled( enable );
    if( ( flags & eButtonJoin ) && joinButton_ )
        joinButton_->setEnabled( enable );
    if( ( flags & eButtonEdit ) && editButton_ )
        editButton_->setEnabled( enable );
    if( ( flags & eButtonApply ) && applyButton_ )
        applyButton_->setEnabled( enable );
    if( ( flags & eButtonDelete ) && deleteButton_ )
        deleteButton_->setEnabled( enable );
    if( ( flags & eButtonUpgrade ) && upgradeButton_ )
        upgradeButton_->setEnabled( enable );
    if( ( flags & eButtonCancel ) && cancelButton_ )
        cancelButton_->setEnabled( enable );
    if( ( flags & eButtonExport ) && exportButton_ )
        exportButton_->setEnabled( enable );
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
    if( ( flags & eButtonApply ) && applyButton_ )
        applyButton_->setText( text );
    if( ( flags & eButtonDelete ) && deleteButton_ )
        deleteButton_->setText( text );
    if( ( flags & eButtonUpgrade ) && upgradeButton_ )
        upgradeButton_->setText( text );
    if( ( flags & eButtonCancel ) && cancelButton_ )
        cancelButton_->setText( text );
    if( ( flags & eButtonExport ) && exportButton_ )
        exportButton_->setText( text );
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
