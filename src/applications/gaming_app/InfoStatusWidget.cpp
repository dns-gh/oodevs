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
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/tools.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/GLToolColors.h"
#include "gaming/Attributes.h"
#include "gaming/HumanFactors.h"
#include "gaming/Reinforcements.h"
#include "icons.h"

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
            if( painter->begin( this ) )
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
InfoStatusWidget::InfoStatusWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, const gui::EntitySymbols& icons )
    : Q3VBox       ( parent, "InfoStatusWidget" )
    , controllers_( controllers )
    , profile_    ( profile )
    , icons_      ( icons )
    , selected_   ( controllers )
    , logo_       ( "resources/images/gui/logo32x32.png" )
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
        icon_->setPixmap( logo_ );
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
    const QPixmap pixmap = QPixmap::fromImage( background_.smoothScale( ev->size() ) );
    setPaletteBackgroundPixmap( pixmap );
    name_->setBackgroundPixmap( pixmap );
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
        if( const Attributes* attributes = selected_->Retrieve< Attributes >() )
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
    icon_->setPixmap( logo_ );
    reinforced_->hide();
    reinforcing_->hide();
    experience_->setPixmap( QPixmap() );
    morale_->hide();
    tiredness_->hide();
    lifeBar_->hide();
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::DrawCrowdChartPie
// Created: JSR 2013-09-27
// -----------------------------------------------------------------------------
void InfoStatusWidget::DrawCrowdChartPie( QPixmap& pixmap, const kernel::Population_ABC& population )
{
    const unsigned int healthy = population.GetHealthyHumans();
    const unsigned int contaminated = population.GetContaminatedHumans();
    const unsigned int wounded = population.GetWoundedHumans();
    const unsigned int dead = population.GetDeadHumans();

    // QPainter::drawPie is specified in 1/16th of a degree
    const float ratio = 16.f * 360 / ( healthy + contaminated + wounded + dead );

    const int healthySpanAngle      = - static_cast< int >( healthy * ratio );
    const int contaminatedSpanAngle = - static_cast< int >( contaminated * ratio );
    const int woundedSpanAngle      = - static_cast< int >( wounded * ratio );
    const int deadSpanAngle         = - static_cast< int >( dead * ratio );

    QPainter painter( &pixmap );
    painter.setPen( Qt::NoPen );
    int angleStart = 90 * 16;
    QRect r = pixmap.rect();
    r.setSize( QSize( r.width() - 1, r.height() - 1 ) );
    // healthy
    painter.setBrush( QColor( controllers_.options_.GetOption( "Color/Healthy" ).To< QString >() ) );
    painter.drawPie( r, angleStart, healthySpanAngle );
    angleStart += healthySpanAngle;
    // contaminated
    painter.setBrush( QColor( controllers_.options_.GetOption( "Color/Contaminated" ).To< QString >() ) );
    painter.drawPie( r, angleStart, contaminatedSpanAngle );
    angleStart += contaminatedSpanAngle;
    // wounded
    painter.setBrush( QColor( controllers_.options_.GetOption( "Color/Wounded" ).To< QString >() ) );
    painter.drawPie( r, angleStart, woundedSpanAngle );
    angleStart += woundedSpanAngle;
    // dead
    painter.setBrush( QColor( controllers_.options_.GetOption( "Color/Dead" ).To< QString >() ) );
    painter.drawPie( r, angleStart, deadSpanAngle );

    painter.setPen( QColor( Qt::black ) );
    painter.setBrush( Qt::NoBrush );
    painter.drawArc( r, 0, 16 * 360 );
    painter.end();
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetIcon
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetIcon()
{
    if( selected_ && selected_->GetTypeName() == kernel::Population_ABC::typeName_ )
    {
        QPixmap pixmap( 128, 128 );
        pixmap.fill( Qt::transparent );
        const kernel::Entity_ABC* entity = static_cast< const kernel::Entity_ABC* >( selected_ );
        DrawCrowdChartPie( pixmap, static_cast< const kernel::Population_ABC& >( *entity ) );
        pixmap = pixmap.scaled( QSize( 64, 64 ), Qt::KeepAspectRatio, Qt::SmoothTransformation );
        icon_->setPixmap( pixmap );
    }
    else
    {
        QImage img;
        if( selected_ )
            img = icons_.GetSymbol( *selected_, QSize( 64, 64 ) );
        if( img.isNull() )
            QTimer::singleShot( 200, this, SLOT( SetIcon() ) );
        else if( img.width() > 1 && img.height() > 1 )
            icon_->setPixmap( QPixmap::fromImage( img ) );
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
        experience_->setPixmap( experienced_ );
    else if( experience == eUnitExperience_Veteran )
        experience_->setPixmap( veteran_ );
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
    morale_->setVisible( morale != eUnitMorale_Bon );
    if( morale == eUnitMorale_Fanatique )
        morale_->setPixmap( boost_ );
    else if( morale == eUnitMorale_Moyen )
        morale_->setPixmap( warning_ );
    else if( morale == eUnitMorale_Mauvais )
        morale_->setPixmap( error_ );
    QToolTip::add( morale_, tr( "Morale: " ) + tools::ToString( morale ) );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetTiredness
// Created: SBO 2008-07-08
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetTiredness( const HumanFactors& humans )
{
    const E_UnitTiredness tiredness = humans.GetTiredness();
    tiredness_->setVisible( tiredness != eUnitTiredness_Normal );
    if( tiredness == eUnitTiredness_Fatigue )
        tiredness_->setPixmap( warning_ );
    else if( tiredness == eUnitTiredness_Epuise )
        tiredness_->setPixmap( error_ );
    QToolTip::add( tiredness_, tr( "Tiredness: " ) + tools::ToString( tiredness ) );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::SetReinforcements
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
void InfoStatusWidget::SetReinforcements( const Reinforcements& reinforcements )
{
    reinforcing_->setVisible( reinforcements.reinforced_ != 0 );
    if( reinforcements.reinforced_ )
        QToolTip::add( reinforcing_, tr( "Reinforcing unit '%1'." ).arg( reinforcements.reinforced_->GetName() ) );
    reinforced_->setVisible( !reinforcements.reinforcements_.empty() );
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
void InfoStatusWidget::NotifyUpdated( const Attributes& element )
{
    if( !selected_ || selected_->Retrieve< Attributes >() != &element )
        return;
    SetLifeBar( element );
    SetName( &element );
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
// Created: JSR 2013-09-26
// -----------------------------------------------------------------------------
void InfoStatusWidget::NotifyUpdated( const kernel::Population_ABC& element )
{
    if( selected_ == &element )
        SetIcon();
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

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::OptionChanged
// Created: JSR 2013-09-27
// -----------------------------------------------------------------------------
void InfoStatusWidget::OptionChanged( const std::string& name, const kernel::OptionVariant& )
{
    if( selected_ && selected_->GetTypeName() == kernel::Population_ABC::typeName_
        && ( name == "Color/Healthy" || name == "Color/Wounded" || name == "Color/Contaminated" || name == "Color/Dead" ) )
        SetIcon();
}

// -----------------------------------------------------------------------------
// Name: InfoStatusWidget::NotifyUpdated
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void InfoStatusWidget::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    if( selected_ != &entity )
        return;
    selected_ = 0;
    NotifySelected( &entity );
}
