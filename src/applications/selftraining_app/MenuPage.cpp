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
#include <qlayout.h>
#include <qpainter.h>

namespace
{
    class TransparentContainer : public QHBox
    {
    public:
        explicit TransparentContainer( QWidget* parent )
            : QHBox( parent )
            , mask_( "resources/images/selftraining/menu-mask.png" )
        {
            setFixedSize( 800, 112 );
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
// Name: MenuPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuPage::MenuPage( QWidgetStack* pages, const QString& title )
    : Page_ABC( pages )
{

    QVBox* box = new QVBox(this);
    box->setSpacing( 5 );    
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    // title 
    QFont titleFont( "Century Gothic", 18, QFont::Bold );  
    titleFont.setItalic( true ) ; 
    title_ = new QLabel( title, box ) ; 
    title_->setBackgroundOrigin( QWidget::WindowOrigin );
    title_->setFont( titleFont ); 
    title_->setAlignment( Qt::AlignCenter ) ; 
    QWidget* spacer = new QWidget( box ); 
    spacer->setMinimumHeight(40); 
    spacer->setBackgroundOrigin( QWidget::WindowOrigin );
    // main container 
    container_ = new TransparentContainer( box);
    // subtitle 
    QFont subTitleFont( "Century Gothic", 12, QFont::Bold );  
    subTitleFont.setItalic( true ) ; 
    subTitle_ = new QLabel( box ) ; 
    subTitle_->setBackgroundOrigin( QWidget::WindowOrigin );
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
void MenuPage::AddLink( const QString& title, Page_ABC& page, bool enabled, const QString& subTitle, const char* slot /*= 0*/ )
{
    MenuButton* button = new MenuButton( title, container_, enabled);
    subTitles_[ button ] = subTitle ; 
    connect( button, SIGNAL( clicked() ), &page, slot ? slot : SLOT( show() ) );
    connect( button, SIGNAL( Selected( MenuButton* ) ), this, SLOT( OnSelectedItem( MenuButton* ) ) );
    connect( button, SIGNAL( UnSelected( MenuButton* ) ), this, SLOT( OnUnSelectedItem( MenuButton* ) ) );
}

// -----------------------------------------------------------------------------
// Name: MenuPage::OnSelectedItem
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void MenuPage::OnSelectedItem( MenuButton* button )
{
    subTitle_->setText( subTitles_[ button ] ) ; 
}

// -----------------------------------------------------------------------------
// Name: MenuPage::OnSelectedItem
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void MenuPage::OnUnSelectedItem( MenuButton* /* button */ )
{
    subTitle_->setText( "" ) ; 
}
