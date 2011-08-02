// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::DensityWidget */

#include "clients_gui_pch.h"
#include "DensityWidget.h"
#include "moc_DensityWidget.cpp"
#include "ColorButton.h"
#include "GradientButton.h"
#include "Painter_ABC.h"
#include "Gradient.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include <boost/lexical_cast.hpp>

using namespace gui;

namespace
{
    class Painter : public Painter_ABC
    {
    public:
                 Painter()
        {
            // NOTHING
        }
        virtual ~Painter()
        {
            // NOTHING
        }
        virtual void Draw( QPainter& painter, unsigned int pourcentage, int x, int y ) const
        {
            QFont font( "Normal", 7, QFont::Light );
            painter.setFont( font );
            int position = pourcentage > 50 ? x - 12 : x;
            painter.drawText( position, y, boost::lexical_cast< std::string >( pourcentage ).c_str() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: GradientWidget constructor
// Created: LGY 2011-06-01
// -----------------------------------------------------------------------------
DensityWidget::DensityWidget( QWidget* parent, kernel::Controllers& controllers, const std::string& category )
    : Q3VBox( parent, "DensityWidget" )
    , category_   ( category )
    , pPainter_   ( new Painter() )
    , controllers_( controllers )
    , options_    ( controllers.options_ )
    , blockLoaded_( false )
    , minLoaded_  ( false )
    , maxLoaded_  ( false )
{
    setMaximumHeight( 150 );

    Q3HBox* valueBox = new Q3HBox( this );
    valueBox->setSpacing( 4 );
    new QLabel( "Min : ", valueBox );

    min_ = new QLineEdit( valueBox );
    min_->setValidator( new QDoubleValidator( 0., 100.f, 2, valueBox ) );
    min_->setText( "0" );
    new QLabel( "Max : ", valueBox );
    max_ = new QLineEdit( valueBox );
    max_->setValidator( new QDoubleValidator( 0., 100.f, 2, valueBox ) );
    max_->setText( "1" );

    Q3HBox* graphicBox = new Q3HBox( this );
    graphicBox->layout()->setAlignment( Qt::AlignCenter );
    graphicBox->setMaximumHeight( 100 );
    densityEditor_ = new GradientButton( graphicBox, *pPainter_, false, Qt::green, Qt::red );
    color_ = new ColorButton( graphicBox );
    color_->setMaximumHeight( 30 );
    Q3HBox* unoccupiedBox = new Q3HBox( this );
    QLabel* label = new QLabel( unoccupiedBox );
    label->setText( tr( "Unoccupied:" ) );
    unoccupiedColor_ = new ColorButton( unoccupiedBox );
    unoccupiedColor_->setMaximumHeight( 30 );
    QPushButton* button = new QPushButton( tr( "Reset" ), this );

    connect( densityEditor_, SIGNAL( SelectionChanged( const QColor& ) ), SLOT( OnSelectionChanged( const QColor& ) ) );
    connect( densityEditor_, SIGNAL( GradientChanged( Gradient& ) ), SLOT( OnGradientEdited( Gradient& ) ) );
    connect( color_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChanged( const QColor& ) ) );
    connect( unoccupiedColor_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnUnoccupiedColorChanged( const QColor& ) ) );
    connect( button, SIGNAL( clicked() ), SLOT( Reset() ) );
    connect( min_, SIGNAL( textChanged( const QString& ) ), SLOT( OnMinChanged( const QString& ) ) );
    connect( max_, SIGNAL( textChanged( const QString& ) ), SLOT( OnMaxChanged( const QString& ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget destructor
// Created: LGY 2011-06-01
// -----------------------------------------------------------------------------
DensityWidget::~DensityWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OnSelectionChanged
// Created: LGY 2011-01-06
// -----------------------------------------------------------------------------
void DensityWidget::OnSelectionChanged( const QColor& color )
{
    color_->SetColor( color );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OnColorChanged
// Created: LGY 2011-01-06
// -----------------------------------------------------------------------------
void DensityWidget::OnColorChanged( const QColor& color )
{
    densityEditor_->SetSelectedColor( color );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OnUnoccupiedColorChanged
// Created: LDC 2011-03-22
// -----------------------------------------------------------------------------
void DensityWidget::OnUnoccupiedColorChanged( const QColor& color )
{
    options_.Change( category_ + "/unoccupied", color.name() );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OnGradientEdited
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void DensityWidget::OnGradientEdited( Gradient& gradient )
{
    gradient.SetName( "urbanBlock" );
    gradient.Save( options_, category_ + "/" );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::Reset
// Created: LGY 2011-01-12
// -----------------------------------------------------------------------------
void DensityWidget::Reset()
{
    Gradient gradient;
    gradient.AddColor( 0, Qt::green );
    gradient.AddColor( 1, Qt::red );
    densityEditor_->LoadGradient( gradient );
    min_->setText( "0" );
    max_->setText( "1" );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OnMinChanged
// Created: LGY 2011-01-12
// -----------------------------------------------------------------------------
void DensityWidget::OnMinChanged( const QString& value )
{
    if( !value.isEmpty() )
        options_.Change( category_ + "/min", value.toFloat() );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OnMaxChanged
// Created: LGY 2011-01-12
// -----------------------------------------------------------------------------
void DensityWidget::OnMaxChanged( const QString& value )
{
    if( !value.isEmpty() )
        options_.Change( category_ + "/max", value.toFloat() );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OptionChanged
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void DensityWidget::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( !blockLoaded_ && name == category_ + "/urbanBlock"  )
    {
        blockLoaded_ = true;
        Gradient gradient;
        const QString colors = value.To< QString >();
        gradient.LoadValues( colors );
        densityEditor_->LoadGradient( gradient );
    }
    else if( !minLoaded_ && name == category_ + "/min"  )
    {
        minLoaded_ = true;
        min_->setText( boost::lexical_cast< std::string >( value.To< float >() ).c_str() );
    }
    else if( !maxLoaded_ && name == category_ + "/max"  )
    {
        maxLoaded_ = true;
        max_->setText( boost::lexical_cast< std::string >( value.To< float >() ).c_str() );
    }
}
