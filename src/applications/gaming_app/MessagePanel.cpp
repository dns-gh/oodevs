// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MessagePanel.h"
#include "moc_MessagePanel.cpp"
#include "gaming/Command.h"
#include "gaming/CommandHandler.h"
#include "gaming/CommandPublisher.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/ItemFactory_ABC.h"
#include "clients_gui/Tools.h"
#include <string>

// -----------------------------------------------------------------------------
// Name: MessagePanel constructor
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
MessagePanel::MessagePanel( QMainWindow* mainWindow, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler, gui::ItemFactory_ABC& factory )
    : gui::RichToolBar( controllers, mainWindow, "MessagePanel", tools::translate( "MessagePanel", "Messages" ) )
    , mainWindow_( mainWindow )
    , handler_( handler )
    , factory_( factory )
    , publisher_( new CommandPublisher( controllers, publisher ) )
    , isVisible_( false )
{
    setObjectName( "message" );
    setPaletteBackgroundColor( QColor( 255, 255, 225 ) );
    setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    setAllowedAreas( Qt::TopToolBarArea );

    //text layout
    text_ = factory_.CreateLabel();
    QFont font( text_->font() );
    font.setPixelSize( 14 );
    text_->setFont( font );
    text_->setTextFormat( Qt::RichText );
    text_->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );

    //buttons layout
    buttons_ = new QWidget();
    QHBoxLayout* buttonsLayout = new QHBoxLayout( buttons_ );
    buttonsLayout->setMargin( 5 );
    buttonsLayout->setSizeConstraint( QLayout::SetMaximumSize );
    buttons_->hide();

    //global layout
    QWidget* box = new QWidget();
    QHBoxLayout* boxLayout = new QHBoxLayout( box );
    boxLayout->addWidget( text_ );
    boxLayout->addWidget( buttons_ );
    boxLayout->setAlignment( text_, Qt::AlignLeft );
    boxLayout->setAlignment( buttons_, Qt::AlignRight );
    boxLayout->setStretch( 0, 100 );

    addWidget( box );

     handler_.Register( "display", *this );
     handler_.Register( "prompt", *this );

     toggleViewAction()->setCheckable( false );
     toggleViewAction()->setChecked( false );
     toggleViewAction()->setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: MessagePanel destructor
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
MessagePanel::~MessagePanel()
{
    handler_.Unregister( "display", *this );
    handler_.Unregister( "prompt", *this );
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::NotifyModeChanged
// Created: NPT 2012-11-27
// -----------------------------------------------------------------------------
void MessagePanel::NotifyModeChanged( int /*newMode*/, bool /*useDefault*/, bool /*firstChangeToSavedMode*/ )
{
    setVisible( isVisible_ );
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::Receive
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::Receive( const Command& command )
{
    if( command.Name() == "display" )
    {
        if( command.ArgumentCount() == 0 )
        {
            Clear();
            return;
        }
        ClearButtons();
        Display( command.Argument( 1 ).c_str() );
    }
    else if( command.Name() == "prompt" && command.ArgumentCount() > 2 )
    {
        ClearButtons();
        activePrompt_ = command.Argument( 1 );
        Display( command.Argument( 2 ).c_str() );
        const QStringList buttons = QStringList::split( '|', command.Argument( 3 ).c_str() );
        for( QStringList::const_iterator it = buttons.begin(); it != buttons.end(); ++it )
            AddButton( *it );
        buttons_->setVisible( true );
    }
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::Display
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::Display( const QString& message )
{
    QString formatted( message );
    formatted.replace( '\n', "<br>" );
    text_->setText( formatted );
    isVisible_ = true;
    setVisible( isVisible_ );
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::AddButton
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::AddButton( const QString& title )
{
    QPushButton* button = new QPushButton( title, buttons_ );
    button->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    QHBoxLayout * buttonLayout = static_cast< QHBoxLayout* >( buttons_->layout() );
    buttonLayout->addWidget( button );
    connect( button, SIGNAL( clicked( bool ) ), this, SLOT( OnButtonPressed() ) );
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::Clear
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::Clear()
{
    Display( "" );
    ClearButtons();
    isVisible_ = false;
    setVisible( isVisible_ );
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::ClearButtons
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::ClearButtons()
{
    QList< QAbstractButton* > children = buttons_->findChildren< QAbstractButton* >( QString() );
    for( QList< QAbstractButton* >::const_iterator it = children.begin(); it != children.end(); ++it )
    {
        buttons_->layout()->remove( *it );
        (*it)->deleteLater();
    }
    buttons_->setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::OnButtonPressed
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::OnButtonPressed()
{
    QAbstractButton* button = dynamic_cast< QAbstractButton* >( QObject::sender() );
    if( button )
    {
        const std::string message = "/choose " + activePrompt_ + " \"" + button->text().toStdString() + "\"";
        publisher_->Send( "", message );
    }
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::sizeHint
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
QSize MessagePanel::sizeHint() const
{
    return QSize( mainWindow_->width(), std::max( text_->height(), 40 ) );
}
