// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoStatusWidget.h"
#include "moc_InfoStatusWidget.cpp"
#include "gaming/Attributes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_gui/SymbolIcons.h"
#include "icons.h"
#include <qpainter.h>
#include <qprogressbar.h>

namespace
{
    class LifeBar : public QProgressBar
    {
    public:
        explicit LifeBar( QWidget* parent )
            : QProgressBar( 100, parent )
        {
            setCenterIndicator( true );
            setPercentageVisible( true );
            setIndicatorFollowsStyle( false );
            setProgress( 0 );
        }
        virtual ~LifeBar() {}

    protected:
        void drawContents( QPainter* painter )
        {
            const QRect bar = contentsRect();
            painter->fillRect( bar, QColor( 255, 100, 100 ) );
            QRect progBar = bar;
            progBar.setRight( bar.left() + float( bar.width() ) * float( progress() ) / 100.f );
            QColor color;
            color.setHsv( progress(), 130, 255 );
            painter->fillRect( progBar, color );
            QRect bbox = painter->boundingRect( bar, Qt::AlignCenter, progressString() );
            painter->drawText( bbox.left(), bbox.bottom(), progressString() );
            painter->drawRect( bar );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget constructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoStatusWidget::InfoStatusWidget( QWidget* parent, kernel::Controllers& controllers, gui::SymbolIcons& icons )
    : QVBox( parent )
    , controllers_( controllers )
    , selected_( controllers )
    , icons_( icons )
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );
    setFixedWidth( 150 );

    QHBox* box = new QHBox( this );
    box->layout()->setAlignment( Qt::AlignRight );
    box->setMaximumHeight( 20 );
    box->setBackgroundColor( Qt::white );

    gotoParent_ = new QPushButton( box );
    gotoParent_->setPixmap( MAKE_PIXMAP( parent ) );
    gotoParent_->setDisabled( true );
    gotoParent_->setFixedSize( 20, 20 );
    gotoParent_->setFlat( true );
    gotoParent_->setBackgroundColor( Qt::white );
    QToolTip::add( gotoParent_, tr( "Goto parent unit" ) );

    icon_ = new QLabel( this );
    icon_->setPixmap( MAKE_PIXMAP( csword ) );
    icon_->setAlignment( Qt::AlignCenter );
    icon_->setBackgroundColor( Qt::white );

    name_ = new QLabel( this );
    QFont font;
    font.setBold( true );
    name_->setFont( font );
    name_->setFixedSize( 150, 40 );
    name_->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
    name_->setBackgroundColor( Qt::white );

    box = new QHBox( this );
    box->setFixedHeight( 20 );
    box->setBackgroundColor( Qt::white );
    lifeBar_ = new LifeBar( box );
    lifeBar_->setFont( font );
    lifeBar_->setFixedSize( 150, 20 );
    
    connect( gotoParent_, SIGNAL( clicked() ), SLOT( GotoParent() ) );
    SetDefault();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget destructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoStatusWidget::~InfoStatusWidget()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::NotifySelected
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
void InfoStatusWidget::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( selected_ != entity )
    {
        selected_ = entity;
        SetDefault();
        if( !selected_ )
            return;
        const kernel::TacticalHierarchies* hierarchies = selected_->Retrieve< kernel::TacticalHierarchies >();
        gotoParent_->setDisabled( !hierarchies || !hierarchies->GetSuperior() );
        if( hierarchies )
            SetIcon( *hierarchies );
        if( const Attributes* attributes = static_cast< const Attributes* >( selected_->Retrieve< kernel::Attributes_ABC >() ) )
        {
            SetLifeBar( *attributes );
            SetName( *attributes );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetDefault
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetDefault()
{
    name_->setPaletteForegroundColor( Qt::black );
    name_->setText( selected_ ? selected_->GetName() : "" );
    icon_->setPixmap( MAKE_PIXMAP( csword ) );
    lifeBar_->hide();
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetIcon
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetIcon( const kernel::TacticalHierarchies& hierarchies )
{
    const std::string symbolName = hierarchies.GetSymbol();
    const std::string levelName  = hierarchies.GetLevel();
    if( ! symbolName.empty() || ! levelName.empty() )
    {
        QImage img;
        img = icons_.GetSymbol( symbolName, levelName );
        img = img.smoothScale( 64, 64, QImage::ScaleMax );
        icon_->setPixmap( img );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetLifeBar
// Created: SBO 2007-02-07
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetLifeBar( const Attributes& attributes )
{
    lifeBar_->show();
    lifeBar_->setProgress( attributes.nRawOpState_ );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetName
// Created: SBO 2007-02-07
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetName( const Attributes& attributes )
{
    QColor color( Qt::black );
    if( attributes.bDead_ )
        color = Qt::red;
    else if( attributes.bNeutralized_ )
        color = Qt::gray;
    name_->setPaletteForegroundColor( color );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::NotifyUpdated
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
void InfoStatusWidget::NotifyUpdated( const kernel::Attributes_ABC& element )
{
    if( !selected_ || selected_->Retrieve< kernel::Attributes_ABC >() != &element )
        return;
    const Attributes& attributes = static_cast< const Attributes& >( element );
    SetLifeBar( attributes );
    SetName( attributes );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::GotoParent
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
void InfoStatusWidget::GotoParent()
{
    if( !selected_ )
        return;
    if( const kernel::TacticalHierarchies* hierarchies = selected_->Retrieve< kernel::TacticalHierarchies >() )
        if( const kernel::Entity_ABC* parent = hierarchies->GetSuperior() )
            controllers_.actions_.Select( *parent );
}
