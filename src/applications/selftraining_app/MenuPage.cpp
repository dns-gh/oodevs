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
    class TransparentContainer : public Q3HBox
    {
    public:
        explicit TransparentContainer( QWidget* parent )
            : Q3HBox( parent )
            , mask_( "resources/images/selftraining/menu-mask.png" )
        {
            setFixedSize( 800, 112 );
        }
    protected:
        virtual void drawContents( QPainter* painter )
        {
            painter->drawImage( frameRect(), mask_ );
            Q3HBox::drawContents( painter );
        }

    private:
        QImage mask_;
    };
}

// -----------------------------------------------------------------------------
// Name: MenuPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuPage::MenuPage( Q3WidgetStack* pages, Page_ABC& previous, unsigned short buttonFlags, const QString& title )
    : Page_ABC( pages, previous, buttonFlags )
{
    Q3VBox* box = new Q3VBox(this);
    AddContent( box );
    box->setSpacing( 5 );
    // title
    QFont titleFont( "Century Gothic", 18, QFont::Bold );
    titleFont.setItalic( true ) ;
    title_ = new QLabel( title, box ) ;
    title_->setFont( titleFont );
    title_->setAlignment( Qt::AlignCenter ) ;
    QWidget* spacer = new QWidget( box );
    spacer->setMinimumHeight( 40 );
    // main container
    container_ = new TransparentContainer( box);

    // subtitle
    QFont subTitleFont( "Century Gothic", 12, QFont::Bold );
    subTitleFont.setItalic( true );
    subTitle_ = new QLabel( box );
    subTitle_->setMargin( 5 );
    subTitle_->setFont( subTitleFont);

    layout()->setAlignment( Qt::AlignCenter );
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
    MenuButton* button = new MenuButton( container_ );
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
