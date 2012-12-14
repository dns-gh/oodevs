// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "MenuPage.h"
#include "moc_MenuPage.cpp"
#include "MenuButton.h"

namespace
{
    class TransparentContainer : public QWidget
    {
    public:
        explicit TransparentContainer( QWidget* parent )
            : QWidget( parent )
            , mask_( "resources/images/selftraining/menu-mask.png" )
        {
            setFixedSize( 800, 112 );
            QHBoxLayout* layout = new QHBoxLayout( this ); 
            layout->setMargin( 5 );
        }
    protected:
        virtual void paintEvent( QPaintEvent* /*e*/ )
        {
            QRect r = rect();
            QPainter painter( this );
            painter.drawImage( rect(), mask_ );
        }

    private:
        QImage mask_;
    };
}

// -----------------------------------------------------------------------------
// Name: MenuPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuPage::MenuPage( QStackedWidget* pages, Page_ABC& previous, unsigned short buttonFlags, const QString& title )
    : Page_ABC( pages, previous, buttonFlags )
{
    QWidget* box = new QWidget( this );
    AddContent( box );

    // title
    QFont titleFont( "Century Gothic", 18, QFont::Bold );
    titleFont.setItalic( true );
    title_ = new QLabel( title );
    title_->setFont( titleFont );
    title_->setAlignment( Qt::AlignCenter ) ;

    QWidget* spacer = new QWidget();
    spacer->setMinimumHeight( 40 );

    // main container
    container_ = new TransparentContainer( box );

    // subtitle
    QFont subTitleFont( "Century Gothic", 12, QFont::Bold );
    subTitleFont.setItalic( true );
    subTitle_ = new QLabel();
    subTitle_->setMargin( 5 );
    subTitle_->setFont( subTitleFont);

    //QWidget* box = new QWidget( pages );
    QVBoxLayout* boxLayout = new QVBoxLayout( box ); 
    boxLayout->setAlignment( Qt::AlignCenter );
    boxLayout->addWidget( title_ );
    boxLayout->addWidget( spacer );
    boxLayout->addWidget( container_ );
    boxLayout->addWidget( subTitle_ );
    boxLayout->setSpacing( 5 );
    boxLayout->setMargin( 0 );
}

// -----------------------------------------------------------------------------
// Name: MenuPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuPage::~MenuPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MenuPage::AddLink
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuButton* MenuPage::AddLink( Page_ABC& page, bool showOnClick )
{
    MenuButton* button = new MenuButton();
    container_->layout()->addWidget( button );
    if( showOnClick )
        connect( button, SIGNAL( clicked() ), &page, SLOT( show() ) );
    connect( button, SIGNAL( Selected( MenuButton* ) ), this, SLOT( OnSelectedItem( MenuButton* ) ) );
    connect( button, SIGNAL( UnSelected( MenuButton* ) ), this, SLOT( OnUnSelectedItem( MenuButton* ) ) );
    return button;
}

// -----------------------------------------------------------------------------
// Name: MenuPage::SetTextAndSubtitle
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void MenuPage::SetTextAndSubtitle( QPushButton* button, const QString& text, const QString& subTitle, bool isError )
{
    button->setText( text );
    subTitles_[ button ] = Subtitle( subTitle, isError );
}

// -----------------------------------------------------------------------------
// Name: MenuPage::OnSelectedItem
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void MenuPage::OnSelectedItem( MenuButton* button )
{
    Subtitle& sub = subTitles_[ button ];
    subTitle_->setText( sub.title );
    QPalette palette;
    palette.setColor( QPalette::WindowText, sub.error ? Qt::red : Qt::white );
    subTitle_->setPalette( palette );
}

// -----------------------------------------------------------------------------
// Name: MenuPage::OnSelectedItem
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void MenuPage::OnUnSelectedItem( MenuButton* /* button */ )
{
    subTitle_->setText( "" ) ;
}

// -----------------------------------------------------------------------------
// Name: MenuPage::SetCenterTitle
// Created: ABR 2011-12-01
// -----------------------------------------------------------------------------
void MenuPage::SetCenterTitle( const QString& title )
{
    title_->setText( title );
}
