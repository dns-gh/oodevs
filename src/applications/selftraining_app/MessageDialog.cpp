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
#include "clients_kernel/Tools.h"

namespace
{
    class TransparentContainer : public QWidget
    {
    public:
        explicit TransparentContainer()
            : mask_( "resources/images/selftraining/menu-mask.png" )
        {
            setFixedSize( 800, 300 );
        }
    protected:
        virtual void paintEvent( QPaintEvent* /*e*/ )
        {
            QPainter painter( this );
            painter.drawImage( rect(), mask_ );
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
    : QDialog( parent, Qt::Dialog )
{
    setWindowTitle( title );
    setModal( true );
    if( parent )
        setPalette( parent->palette() );

    QFont font( "Century Gothic", 13, QFont::Bold );
    font.setItalic( true );
    setFont( font );

    QLabel* label = new QLabel();
    label->setText( message );
    label->setAlignment(Qt::AlignCenter );
    label->setMinimumHeight( 50 );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    CreateButton( buttonLayout, button1 );
    if( button2 != -1 )
        CreateButton( buttonLayout, button2 );

    QWidget* spacer = new QWidget();
    spacer->setMinimumHeight( 50 );

    QVBoxLayout* vboxLayout = new QVBoxLayout( this );
    vboxLayout->setSpacing( 10 );
    vboxLayout->addWidget( label );
    vboxLayout->addLayout( buttonLayout );
    vboxLayout->addWidget( spacer );
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
void MessageDialog::CreateButton( QLayout* parent, int button )
{
    QPushButton* buttonWidget = new MenuButton();
    buttonWidget->setStyleSheet( "color : #FFFFFF" ); // $$$$ ABR 2012-07-20: Still ugly, but at least readable
    parent->addWidget( buttonWidget );

    switch( button )
    {
    case QMessageBox::Ok:
        buttonWidget->setText( tools::translate( "MessageDialog", "OK" ) );
        connect( buttonWidget, SIGNAL( clicked() ), this, SLOT( OnYes() ) );
        break;
    case QMessageBox::Yes:
        buttonWidget->setText( tools::translate( "MessageDialog", "Yes" ) );
        connect( buttonWidget, SIGNAL( clicked() ), this, SLOT( OnYes() ) );
        break;
    case QMessageBox::No:
        buttonWidget->setText( tools::translate( "MessageDialog", "No" ) );
        connect( buttonWidget, SIGNAL( clicked() ), this, SLOT( OnNo() ) );
        break;
    default:
        assert( false );
    }
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