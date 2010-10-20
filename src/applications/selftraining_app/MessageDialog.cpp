// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "MessageDialog.h"
#include "moc_MessageDialog.cpp"
#include "MenuButton.h"
#include "clients_gui/Tools.h"

namespace
{
    class TransparentContainer : public QHBox
    {
    public:
        explicit TransparentContainer( QWidget* parent )
            : QHBox( parent )
            , mask_( "resources/images/selftraining/menu-mask.png" )
        {
            setFixedSize( 800, 300 );
        }
    protected:
        virtual void drawContents( QPainter* painter )
        {
            painter->drawImage( frameRect(), mask_ );
            QHBox::drawContents( painter );
        }

    private:
        QImage mask_;
    };
}

// -----------------------------------------------------------------------------
// Name: MessageDialog constructor
// Created: RDS 2008-08-20
// -----------------------------------------------------------------------------
MessageDialog::MessageDialog( QWidget* parent, const QString& title, const QString& message, int button1, int button2 )
    : QDialog( parent, title, true, WStyle_DialogBorder )
{
    setCaption( title );
    if( parent )
        setPalette( parent->palette() );

    QFont font( "Century Gothic", 13, QFont::Bold );
    font.setItalic( true );
    setFont( font );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );

    QVBox* vbox = new QVBox( this );
    vbox->setBackgroundOrigin( QWidget::WindowOrigin );
    vbox->setSpacing( 10 );
    QLabel* label = new QLabel( vbox );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    label->setText( message );
    label->setAlignment( Qt::AlignCenter | Qt::WordBreak );
    label->setMinimumHeight( 50 );
    QHBox* hbox = new QHBox( vbox );

    CreateButton( hbox, button1 );
    CreateButton( hbox, button2 );

    mainLayout->addWidget( vbox );

    QWidget* spacer = new QWidget( vbox );
    spacer->setMinimumHeight( 50 );
    spacer->setBackgroundOrigin( QWidget::WindowOrigin );
}

// -----------------------------------------------------------------------------
// Name: MessageDialog destructor
// Created: RDS 2008-08-20
// -----------------------------------------------------------------------------
MessageDialog::~MessageDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessageDialog::CreateButton
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
QButton* MessageDialog::CreateButton( QWidget* parent, int button )
{
    QButton* buttonWidget = 0;
    switch( button )
    {
    case QMessageBox::Ok:
        buttonWidget = new MenuButton( tools::translate( "MessageDialog", "OK" ), parent );
        connect( buttonWidget, SIGNAL( clicked() ), this, SLOT( OnYes() ) );
        break;
    case QMessageBox::Yes:
        buttonWidget = new MenuButton( tools::translate( "MessageDialog", "Yes" ), parent );
        connect( buttonWidget, SIGNAL( clicked() ), this, SLOT( OnYes() ) );
        break;
    case QMessageBox::No:
        buttonWidget = new MenuButton( tools::translate( "MessageDialog", "No" ), parent );
        connect( buttonWidget, SIGNAL( clicked() ), this, SLOT( OnNo() ) );
        break;
    }
    return buttonWidget;
}

// -----------------------------------------------------------------------------
// Name: MessageDialog::OnYes
// Created: RDS 2008-08-20
// -----------------------------------------------------------------------------
void MessageDialog::OnYes()
{
    done( QMessageBox::Yes );
}

// -----------------------------------------------------------------------------
// Name: MessageDialog::OnNo
// Created: RDS 2008-08-20
// -----------------------------------------------------------------------------
void MessageDialog::OnNo()
{
    done( QMessageBox::No );
}

// -----------------------------------------------------------------------------
// Name: MessageDialog::sizeHint
// Created: RDS 2008-08-20
// -----------------------------------------------------------------------------
QSize MessageDialog::sizeHint() const
{
    return QSize( 500, 300 );
}