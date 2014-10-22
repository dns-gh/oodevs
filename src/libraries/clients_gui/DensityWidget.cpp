// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DensityWidget.h"
#include "moc_DensityWidget.cpp"

#include "ColorButton.h"
#include "Gradient.h"
#include "GradientWidget.h"
#include "OptionWidgets.h"
#include "RichPushButton.h"
#include "RichWidget.h"
#include "RichSpinBox.h"
#include "SignalAdapter.h"
#include "SubObjectName.h"
#include "Tools.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"

#include <boost/lexical_cast.hpp>

using namespace gui;

namespace
{
    class OptionDoubleSpinBox : public OptionWidget< RichDoubleSpinBox >
    {
    public:
        OptionDoubleSpinBox( kernel::OptionsController& options,
                              const QString& objectName,
                              const std::string& optionName,
                              QWidget* parent = 0 )
            : OptionWidget< RichDoubleSpinBox >( options, objectName, optionName, parent )
        {
            setRange( 0.f, 100.f );
            setSingleStep( 0.1f );
            setDecimals( 5 );
            gui::connect( this, SIGNAL( valueChanged( double ) ), [=,&options]{
                options.Change( optionName, static_cast< float >( this->value() ) );
            } );
        }
    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value )
        {
            setValue( value.To< float >() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: DensityWidget constructor
// Created: LGY 2011-06-01
// -----------------------------------------------------------------------------
DensityWidget::DensityWidget( kernel::OptionsController& options,
                              const QString& objectName,
                              const std::string& category,
                              QWidget* parent /* = 0 */ )
    : QWidget( parent )
    , options_( options )
    , optionName_( category )
{
    SubObjectName subObject( objectName );
    setMaximumHeight( 170 );

    min_ = new OptionDoubleSpinBox( options, "min", category + "/Min" );
    max_ = new OptionDoubleSpinBox( options, "max", category + "/Max" );

    const auto itemDrawer = []( QPainter& painter, unsigned int pourcentage, int x, int y ) {
            QFont font( "Normal", 7, QFont::Light );
            painter.setFont( font );
            int position = pourcentage > 50 ? x - 12 : x;
            painter.drawText( position, y, boost::lexical_cast< std::string >( pourcentage ).c_str() );
    };
    gradientWidget_ = new GradientWidget( "density-editor", false, itemDrawer );

    auto unoccupiedColor = new OptionColorButton( options, "unoccupiedColor", category + "/Unoccupied" );

    RichPushButton* button = new RichPushButton( "reset", tr( "Reset" ) );

    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setMargin( 0 );
    headerLayout->addWidget( new QLabel( tr( "Min:" ) ) );
    headerLayout->addWidget( min_ );
    headerLayout->addWidget( new QLabel( tr( "Max:" ) ) );
    headerLayout->addWidget( max_ );

    QVBoxLayout* layout  = new QVBoxLayout( this );
    layout->setMargin( 0 );
    layout->addLayout( headerLayout );
    layout->addWidget( gradientWidget_, 1 );
    layout->addWidget( tools::AddLabeledWidget( tr( "Unoccupied:" ), unoccupiedColor ) );
    layout->addWidget( button );

    connect( gradientWidget_, SIGNAL( GradientChanged() ), SLOT( OnGradientEdited() ) );
    gui::connect( button, SIGNAL( clicked() ), [=](){
        *gradient_ = Gradient( gradient_->GetName() );
        gradient_->AddColor( 0, Qt::green );
        gradient_->AddColor( 1, Qt::red );
        gradientWidget_->LoadGradient( gradient_ );
        min_->setValue( 0.f );
        max_->setValue( 1.f );
    } );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget destructor
// Created: LGY 2011-06-01
// -----------------------------------------------------------------------------
DensityWidget::~DensityWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OnGradientEdited
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void DensityWidget::OnGradientEdited()
{
    if( gradient_ )
        gradient_->Save( options_, optionName_ + "/" );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::Load
// Created: ABR 2014-08-06
// -----------------------------------------------------------------------------
void DensityWidget::Load( const kernel::Options& options,
                          const std::shared_ptr< Gradient >& gradient )
{
    max_->setValue( options.Get( optionName_ + "/Max" ).To< float >() );
    min_->setValue( options.Get( optionName_ + "/Min" ).To< float >() );
    gradient_ = gradient;
    gradientWidget_->LoadGradient( gradient_ );
}
