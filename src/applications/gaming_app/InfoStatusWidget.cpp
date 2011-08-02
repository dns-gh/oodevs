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
#include "gaming/HumanFactors.h"
#include "gaming/Reinforcements.h"
#include "gaming/tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_gui/EntitySymbols.h"
#include "icons.h"
#pragma warning( push, 0 )
#include <QtGui/qevent.h>
#pragma warning( pop )


namespace
{
    class LifeBar : public Q3ProgressBar
    {
    public:
        explicit LifeBar( QWidget* parent )
            : Q3ProgressBar( 100, parent )
            , reflection_( new QImage( "resources/images/gaming/lifebarmask.png" ) )
        {
            setCenterIndicator( true );
            setPercentageVisible( true );
            setProgress( 0 );
        }
        virtual ~LifeBar() {}

    private:
        virtual void paintEvent( QPaintEvent* )
        {
            QPainter* painter = new QPainter();
            if ( painter->begin( this ) )
            {
                const QRect bar = contentsRect();
                painter->fillRect( bar, QColor( 255, 100, 100 ) );
                QRect progBar = bar;
                progBar.setRight( bar.left() + int( float( bar.width() ) * float( progress() ) / 100.f ) );
                QColor color;
                color.setHsv( progress(), 255, 200 );
                painter->fillRect( progBar, color );
                QRect bbox = painter->boundingRect( bar, Qt::AlignCenter, progressString() );
                painter->drawText( bbox.left(), bbox.bottom() - 2, progressString() );
                if( reflection_ )
                    painter->drawImage( bar, *reflection_ );
                painter->drawRect( bar );
                painter->end();
            }
        }
        QImage* reflection_;
    };

        class TransparentButton : public QPushButton
    {
    public:
        explicit TransparentButton( QWidget* parent )
            : QPushButton( parent )
        {
            setBackgroundOrigin( QWidget::AncestorOrigin );
        }
        virtual void setPixmap( const QPixmap& pixmap )
        {
            iconSet_ = QIcon( pixmap );
            QPushButton::setPixmap( pixmap );
        }
    protected:
        virtual void drawButton( QPainter* p )
        {
            p->drawPixmap( 0, 0, iconSet_.pixmap( QIcon::Automatic, isEnabled() ? QIcon::Normal : QIcon::Disabled ) );
        }
        QIcon iconSet_;
    };

    class IconLabel : public QLabel
    {
    public:
        IconLabel( const QString& text, QWidget* parent, const QFont& font )
            : QLabel( text, parent )
            , text_( text )
        {
            setFont( font );
            setPaletteForegroundColor( Qt::white );
            setBackgroundOrigin( QWidget::AncestorOrigin );
        }
        virtual void setPixmap( const QPixmap& pixmap )
        {
            pixmap_ = pixmap;
            QLabel::setPixmap( pixmap );
        }

    protected:
        virtual void drawContents( QPainter* painter )
        {
            const int strWidth = painter->fontMetrics().width( text_ );
            const int strHeight = painter->fontMetrics().height();
            painter->drawPixmap( 1, 2, pixmap_ );
            painter->drawText( QRect( pixmap_.width() + 3, 0, strWidth, strHeight ), 0, text_ );
        }

        QString text_;
        QPixmap pixmap_;
    };
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget constructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoStatusWidget::InfoStatusWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : Q3VBox       ( parent, "InfoStatusWidget" )
    , controllers_( controllers )
    , profile_    ( profile )
    , icons_      ( icons )
    , selected_   ( controllers )
    , csword_     ( "resources/images/gui/logo32x32.png" )
    , background_ ( "resources/images/gaming/status_background.png" )
    , boost_      ( "resources/images/gaming/status_boost.png" )
    , warning_    ( "resources/images/gaming/status_warning.png" )
    , error_      ( "resources/images/gaming/status_error.png" )
    , experienced_( "resources/images/gaming/experience_experienced.png" )
    , veteran_    ( "resources/images/gaming/experience_veteran.png" )
{
    setFixedWidth( 150 );
    setAutoFillBackground( true );
    setPalette( QPalette( backgroundRole(), QBrush( background_ ) ) );
    
    Q3HBox* box = new Q3HBox( this );
    box->setBackgroundOrigin( QWidget::AncestorOrigin );
    box->layout()->setAlignment( Qt::AlignRight );
    box->setMaximumHeight( 20 );
    {
        gotoParent_ = new TransparentButton( box );
        gotoParent_->setPixmap( MAKE_PIXMAP( parent ) );
        gotoParent_->setDisabled( true );
        gotoParent_->setFixedSize( 20, 20 );
        QToolTip::add( gotoParent_, tr( "Goto parent unit" ) );
    }

    box = new Q3HBox( this );
    box->setBackgroundOrigin( QWidget::AncestorOrigin );
    {
        Q3VBox* vBox = new Q3VBox( box );
        vBox->setFixedWidth( 30 );
        vBox->setBackgroundOrigin( QWidget::AncestorOrigin );
        reinforced_ = new QLabel( vBox );
        reinforced_->setPixmap( MAKE_PIXMAP( reinforced ) );
        reinforced_->setBackgroundOrigin( QWidget::AncestorOrigin );
        reinforcing_ = new QLabel( vBox );
        reinforcing_->setPixmap( MAKE_PIXMAP( reinforcing ) );
        reinforcing_->setBackgroundOrigin( QWidget::AncestorOrigin );
        icon_ = new QLabel( box );
        icon_->setFixedSize( 90, 65 );
        icon_->setPixmap( csword_ );
        icon_->setAlignment( Qt::AlignCenter );
        icon_->setBackgroundOrigin( QWidget::AncestorOrigin );
        experience_ = new QLabel( box );
        experience_->setFixedWidth( 30 );
        experience_->setAlignment( Qt::AlignCenter );
        experience_->setBackgroundOrigin( QWidget::AncestorOrigin );
    }

    name_ = new QLabel( this );
    QFont font( "Arial", 8, QFont::Bold );
    font.setBold( true );
    name_->setFont( font );
    name_->setFixedSize( 150, 20 );
    name_->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
    name_->setBackgroundOrigin( QWidget::AncestorOrigin );

    box = new Q3HBox( this );
    box->setFixedHeight( 15 );
    box->setBackgroundOrigin( QWidget::AncestorOrigin );
    font.setBold( false );
    {
        morale_ = new IconLabel( tr( "morale" ), box, font );
        tiredness_ = new IconLabel( tr( "tiredness" ), box, font );
    }

    box = new Q3HBox( this );
    box->setFixedHeight( 20 );
    box->setBackgroundOrigin( QWidget::AncestorOrigin );
    lifeBar_ = new LifeBar( box );
    font = QFont( "Arial", 10, QFont::Bold );
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
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::resizeEvent
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void InfoStatusWidget::resizeEvent( QResizeEvent* ev )
{
    QImage img( background_.smoothScale( ev->size() ) );
    setPaletteBackgroundPixmap( QPixmap::fromImage( img ) );
    name_->setBackgroundPixmap( QPixmap::fromImage( img ) );
    Q3VBox::resizeEvent( ev );
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
        const kernel::Entity_ABC* parent = hierarchies ? hierarchies->GetSuperior() : 0;
        gotoParent_->setDisabled( !hierarchies || !parent || !profile_.IsVisible( *parent ) );
        if( hierarchies )
            SetIcon();
        if( const Attributes* attributes = static_cast< const Attributes* >( selected_->Retrieve< kernel::Attributes_ABC >() ) )
        {
            SetLifeBar( *attributes );
            SetName( attributes );
        }
        if( const HumanFactors* humans = static_cast< const HumanFactors* >( selected_->Retrieve< kernel::HumanFactors_ABC >() ) )
            SetHumanFactors( *humans );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetDefault
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetDefault()
{
    SetName( 0 );
    icon_->setPixmap( csword_ );
    reinforced_->hide();
    reinforcing_->hide();
    experience_->setPixmap( QPixmap() );
    morale_->hide();
    tiredness_->hide();
    lifeBar_->hide();
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetIcon
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetIcon()
{
    QImage img;
    if( selected_ )
        img = icons_.GetSymbol( *selected_, QSize( 64, 64 ) );
    if( !img.isNull() )
        icon_->setPixmap( QPixmap::fromImage( img ) );
    else
        QTimer::singleShot( 200, this, SLOT( SetIcon() ) );
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

namespace
{
    QString CutString( QLabel* label, const QString& text )
    {
        const QFontMetrics metrics( label->font() );
        if( metrics.width( text ) > label->width() )
            return text.left( label->width() / metrics.maxWidth() * 4 ) + "...";
        return text;
    }
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetName
// Created: SBO 2007-02-07
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetName( const Attributes* attributes )
{
    QColor color( Qt::white );
    if( attributes )
    {
        if( attributes->bDead_ )
            color = Qt::red;
        else if( attributes->bNeutralized_ )
            color = Qt::gray;
    }
    name_->setPaletteForegroundColor( color );
    name_->setText( selected_ ? CutString( name_, selected_->GetName() ) : "" );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetHumanFactors
// Created: SBO 2008-07-08
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetHumanFactors( const HumanFactors& humans )
{
    SetExperience( humans );
    SetMorale( humans );
    SetTiredness( humans );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetExperience
// Created: SBO 2008-07-08
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetExperience( const HumanFactors& humans )
{
    const E_UnitExperience experience = humans.GetExperience();
    if( experience == eUnitExperience_Experimente )
        experience_->setPixmap( QPixmap::fromImage( experienced_ ) );
    else if( experience == eUnitExperience_Veteran )
        experience_->setPixmap( QPixmap::fromImage( veteran_ ) );
    else
        experience_->setPixmap( QPixmap() );
    QToolTip::add( experience_, tr( "Experience: " ) + tools::ToString( experience ) );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetMorale
// Created: SBO 2008-07-08
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetMorale( const HumanFactors& humans )
{
    const E_UnitMorale morale = humans.GetMorale();
    morale_->setShown( morale != eUnitMorale_Bon );
    if( morale == eUnitMorale_Fanatique )
        morale_->setPixmap( QPixmap::fromImage( boost_ ) );
    else if( morale == eUnitMorale_Moyen )
        morale_->setPixmap( QPixmap::fromImage( warning_ ) );
    else if( morale == eUnitMorale_Mauvais )
        morale_->setPixmap( QPixmap::fromImage( error_ ) );
    QToolTip::add( morale_, tr( "Morale: " ) + tools::ToString( morale ) );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetTiredness
// Created: SBO 2008-07-08
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetTiredness( const HumanFactors& humans )
{
    const E_UnitTiredness tiredness = humans.GetTiredness();
    tiredness_->setShown( tiredness != eUnitTiredness_Normal );
    if( tiredness == eUnitTiredness_Fatigue )
        tiredness_->setPixmap( QPixmap::fromImage( warning_ ) );
    else if( tiredness == eUnitTiredness_Epuise )
        tiredness_->setPixmap( QPixmap::fromImage( error_ ) );
    QToolTip::add( tiredness_, tr( "Tiredness: " ) + tools::ToString( tiredness ) );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetReinforcements
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetReinforcements( const Reinforcements& reinforcements )
{
    reinforcing_->setShown( reinforcements.reinforced_ != 0 );
    if( reinforcements.reinforced_ )
        QToolTip::add( reinforcing_, tr( "Reinforcing unit '%1'." ).arg( reinforcements.reinforced_->GetName() ) );
    reinforced_->setShown( !reinforcements.reinforcements_.empty() );
    if( !reinforcements.reinforcements_.empty() )
    {
        QString tip = tr( "Reinforced by: " );
        for( unsigned int i = 0; i < reinforcements.reinforcements_.size(); ++i )
            tip += "\n" + reinforcements.reinforcements_[i]->GetName();
        QToolTip::add( reinforced_, tip );
    }
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
    SetName( &attributes );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::NotifyUpdated
// Created: SBO 2008-07-08
// -----------------------------------------------------------------------------
void InfoStatusWidget::NotifyUpdated( const kernel::HumanFactors_ABC& element )
{
    if( !selected_ || selected_->Retrieve< kernel::HumanFactors_ABC >() != &element )
        return;
    const HumanFactors& humans = static_cast< const HumanFactors& >( element );
    SetHumanFactors( humans );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::NotifyUpdated
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
void InfoStatusWidget::NotifyUpdated( const Reinforcements& element )
{
    if( !selected_ || selected_->Retrieve< Reinforcements >() != &element )
        return;
    SetReinforcements( element );
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
