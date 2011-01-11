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
    std::string Format( double number )
    {
        std::stringstream s;
        s << std::fixed << std::setprecision( 2 ) << number;
        return s.str();
    }

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
            QFont font( "Normal", 8, QFont::Light );
            painter.setFont( font );
            double convert = static_cast< double >( pourcentage / 100.f );
            int position = pourcentage > 50 ? x - 20 : x;
            painter.drawText( position, y, Format( convert ).c_str() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: GradientWidget constructor
// Created: LGY 2011-06-01
// -----------------------------------------------------------------------------
DensityWidget::DensityWidget( QWidget* parent, kernel::Controllers& controllers )
    : QVBox( parent )
    , pPainter_   ( new Painter() )
    , controllers_( controllers )
    , options_    ( controllers.options_ )
{
    setMaximumHeight( 150 );
    QHBox* box = new QHBox( this );
    box->layout()->setAlignment( Qt::AlignCenter );
    box->setMaximumHeight( 100 );
    densityEditor_ = new GradientButton( box, *pPainter_, false, Qt::green, Qt::red );
    color_ = new ColorButton( box );
    color_->setMaximumHeight( 30 );

    connect( densityEditor_, SIGNAL( SelectionChanged( const QColor& ) ), SLOT( OnSelectionChanged( const QColor& ) ) );
    connect( densityEditor_, SIGNAL( GradientChanged( Gradient& ) ), SLOT( OnGradientEdited( Gradient& ) ) );
    connect( color_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChanged( const QColor& ) ) );

    densityEditor_->Save();
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
// Name: DensityWidget::OnGradientEdited
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void DensityWidget::OnGradientEdited( Gradient& gradient )
{
    gradient.SetName( "urbanBlock" );
    gradient.Save( options_, "Density/" );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OptionChanged
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void DensityWidget::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "Density/urbanBlock" )
    {
        Gradient gradient;
        const QString colors = value.To< QString >();
        gradient.LoadValues( colors );
        densityEditor_->LoadGradient( gradient, true );
    }
}
