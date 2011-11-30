// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoStancesWidget.h"
#include "gaming/Attributes.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;

namespace
{
    class InstallationProgress : public Q3ProgressBar
    {
    public:
        explicit InstallationProgress( QWidget* parent )
            : Q3ProgressBar( 100, parent )
            , reflection_( new QImage( "resources/images/gaming/lifebarmask.png" ) )
        {
            setProgress( 0 );
            setPercentageVisible( false );
        }
        virtual ~InstallationProgress() {}

    private:
        virtual void paintEvent( QPaintEvent* )
        {
            QPainter* painter = new QPainter();
            if( painter->begin( this ) )
            {
                const QRect bar = contentsRect();
                painter->fillRect( bar, QColor( 200, 200, 200 ) );
                QRect progBar = bar;
                progBar.setRight( bar.left() + int( float( bar.width() ) * float( progress() ) / 100.f ) );
                QColor color;
                color.setHsv( 225, progress() * 255 / 100, 200 );
                painter->fillRect( progBar, color );
                if( reflection_ )
                    painter->drawImage( bar, *reflection_ );
                for( int i = 1; i < 4; ++i )
                {
                    const int x = bar.left() + i * width() / 4;
                    painter->drawLine( x, bar.top(), x, bar.bottom() );
                }
                painter->drawRect( bar );
                painter->end();
            }
        }

        QImage* reflection_;
    };

    class MovementProgress : public Q3ProgressBar
    {
    public:
        explicit MovementProgress( QWidget* parent )
            : Q3ProgressBar( 100, parent )
            , reflection_( new QImage( "resources/images/gaming/lifebarmask.png" ) )
        {
            setProgress( 0 );
            setPercentageVisible( false );
        }
        virtual ~MovementProgress() {}

    private:
        virtual void paintEvent( QPaintEvent* )
        {
            QPainter* painter = new QPainter();
            if( painter->begin( this ) )
            {
                static const QColor colors[] = { QColor( 255,  50, 50 ), QColor( 255, 50, 50 )
                                               , QColor( 255, 255, 50 ), QColor( 50, 255, 50 ) };
                const QRect bar = contentsRect();
                painter->fillRect( bar, colors[ progress() / 33 ] );
                if( reflection_ )
                    painter->drawImage( bar, *reflection_ );
                painter->drawRect( bar );
                painter->end();
            }
        }

        QImage* reflection_;
    };
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget constructor
// Created: SBO 2007-02-09
// -----------------------------------------------------------------------------
InfoStancesWidget::InfoStancesWidget( QWidget* parent, kernel::Controllers& controllers )
    : Q3VBox( parent, "InfoStancesWidget" )
    , controllers_( controllers )
    , selected_( controllers )
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    Q3HBox* box = new Q3HBox( this );
    {
        QFont font( "Arial", 7 );
        QLabel* label = new QLabel( tools::translate( "InfoStancesWidget", "Movement" ), box );
        label->setFixedSize( 100, 12 );
        label->setFont( font );
        label = new QLabel( tools::translate( "InfoStancesWidget", "Installation" ), box );
        label->setFixedSize( 150, 12 );
        label->setFont( font );
        label->setAlignment( Qt::AlignRight );
    }
    box = new Q3HBox( this );
    {
        movement_ = new MovementProgress( box );
        movement_->setFixedSize( 50, 8 );
    }
    {
        installation_ = new InstallationProgress( box );
        installation_->setFixedSize( 200, 8 );
    }
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget destructor
// Created: SBO 2007-02-09
// -----------------------------------------------------------------------------
InfoStancesWidget::~InfoStancesWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget::Update
// Created: SBO 2007-02-09
// -----------------------------------------------------------------------------
void InfoStancesWidget::Update( const Attributes& attributes )
{
    show();
    const int installation = attributes.nCurrentPosture_ - eUnitPosture_PosturePosteReflexe;
    movement_->setProgress( std::max< int >( -installation * 33, 0 ) );
    if( installation < 1 )
        installation_->setProgress( 0 );
    else
        installation_->setProgress( std::max< int >( int( float( installation * 100 + attributes.nPostureCompletionPourcentage_ ) / 4.f ), 0 ) );
    QToolTip::add( installation_, tools::translate( "InfoStancesWidget", "Stance '%1' at %2" )
                                    .arg( tools::ToString( attributes.nCurrentPosture_ ) )
                                    .arg( QString::number( attributes.nPostureCompletionPourcentage_ ) + Units::percentage.AsString() ) );
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget::NotifySelected
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void InfoStancesWidget::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( selected_ != entity )
    {
        selected_ = entity;
        hide();
        if( !selected_ )
            return;
        if( const Attributes* attributes = static_cast< const Attributes* >( selected_->Retrieve< kernel::Attributes_ABC >() ) )
            Update( *attributes );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget::NotifyUpdated
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void InfoStancesWidget::NotifyUpdated( const kernel::Attributes_ABC& extension )
{
    if( selected_ && selected_->Retrieve< kernel::Attributes_ABC >() == &extension )
        Update( static_cast< const Attributes& >( extension ) );
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget::sizeHint
// Created: SBO 2008-07-23
// -----------------------------------------------------------------------------
QSize InfoStancesWidget::sizeHint() const
{
    return QSize( 250, 22 );
}
