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
#include "SignalAdapter.h"
#include "SubObjectName.h"
#include "Tools.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"

#include <boost/lexical_cast.hpp>

using namespace gui;

namespace
{
    class OptionDoubleLineEdit : public OptionWidget< RichWidget< QLineEdit > >
    {
    public:
        OptionDoubleLineEdit( kernel::OptionsController& options,
                              const QString& objectName,
                              const std::string& optionName,
                              QWidget* parent = 0 )
            : OptionWidget< RichWidget< QLineEdit > >( options, objectName, optionName, parent )
        {
            setValidator( new QDoubleValidator( 0., 100.f, 2, this ) );
            gui::connect( this, SIGNAL( textChanged( const QString& ) ), [=,&options]{
                options.Change( optionName, text().toFloat() );
            } );
        }
    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value )
        {
            setText( QString::number( value.To< float >() ) );
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
    , gradient_( std::make_shared< Gradient >( "Gradient" ) )
{
    SubObjectName subObject( objectName );
    setMaximumHeight( 170 );

    auto min = new OptionDoubleLineEdit( options, "min", category + "/Min" );
    auto max = new OptionDoubleLineEdit( options, "max", category + "/Max" );

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
    headerLayout->addWidget( min );
    headerLayout->addWidget( new QLabel( tr( "Max:" ) ) );
    headerLayout->addWidget( max );

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
        min->setText( "0" );
        max->setText( "1" );
    } );
    options_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget destructor
// Created: LGY 2011-06-01
// -----------------------------------------------------------------------------
DensityWidget::~DensityWidget()
{
    options_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OnGradientEdited
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void DensityWidget::OnGradientEdited()
{
    gradient_->Save( options_, optionName_ + "/" );
}

// -----------------------------------------------------------------------------
// Name: DensityWidget::OptionChanged
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void DensityWidget::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == optionName_ + "/Gradient" )
    {
        gradient_->LoadValues( value.To< QString >() );
        gradientWidget_->LoadGradient( gradient_ );
    }
}
