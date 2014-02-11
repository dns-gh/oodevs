// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "WeatherWidget.h"
#include "SubObjectName.h"
#include "RichSpinBox.h"
#include "WeatherHelpers.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"
#include "meteo/Meteo.h"
#include "meteo/PHY_Precipitation.h"

using namespace gui;
using namespace kernel;

namespace
{
    class RichDial : public QDial
    {
    public:
        RichDial( QWidget* parent, const QString& imagePath, float sizeRatio, const QString& objectName )
            : QDial( parent )
            , image_    ( imagePath )
            , sizeRatio_( sizeRatio )
        {
            setObjectName( objectName );
            image_.setAlphaBuffer( true );
            imageRatio_ = ( image_.width() > image_.height() )
                ? (float) image_.height() / (float) image_.width()
                : (float) image_.width() / (float) image_.height();
        }

        virtual void paintEvent( QPaintEvent* event )
        {
            Q_UNUSED( event );

            // Painter
            QPalette p = palette();
            QStylePainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setBrush( p.color( QPalette::Dark ) );

            // Dial button
            QStyleOptionSlider option;
            initStyleOption(&option);
            painter.drawComplexControl(QStyle::CC_Dial, option);

            // Windsock
            painter.translate( QPoint( width() / 2, height() / 2 ) );
            QRect surface;
            float surfaceHalfSize = ( width() > height() ) ? static_cast< float >( height() ) * sizeRatio_ / 2.f : static_cast< float >( width() ) * sizeRatio_ / 2.f;
            if( image_.width() > image_.height() )
            {
                painter.rotate( value() - 90 );
                surface = QRect( QPoint( static_cast< int >( -surfaceHalfSize ), static_cast< int >( -surfaceHalfSize * imageRatio_ ) ),
                                 QPoint( static_cast< int >(  surfaceHalfSize ), static_cast< int >(  surfaceHalfSize * imageRatio_ ) ) );
            }
            else
            {
                painter.rotate( value() );
                surface = QRect( QPoint( static_cast< int >( -surfaceHalfSize * imageRatio_ ), static_cast< int >( -surfaceHalfSize ) ),
                                 QPoint( static_cast< int >(  surfaceHalfSize * imageRatio_ ), static_cast< int >(  surfaceHalfSize ) ) );
            }
            painter.drawImage( surface, image_ );
        }

    private:
        QImage image_;
        float sizeRatio_;
        float imageRatio_;
    };
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget constructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
WeatherWidget::WeatherWidget( const QString& objectName, QWidget* parent, const QString& title )
        : RichGroupBox( objectName, title , parent )
{
    gui::SubObjectName subObject( objectName );
    // Speed
    QLabel* windSpeedLabel = new QLabel( tools::translate( "gui::WeatherWidget", "Wind speed:" ), this );
    windSpeed_ = new RichSpinBox( "windSpeed", this, 0, 300, 5 );
    windSpeed_->setSuffix( Units::kilometersPerHour.AsString() );

    // Direction
    QLabel* windDirectionLabel = new QLabel( tools::translate( "gui::WeatherWidget", "Wind direction:" ) );
    windDirection_ = new RichDial( this, "resources/images/gui/windsock.png", 0.8f, "wind-direction" );
    windDirection_->setWrapping( true );
    windDirection_->setRange( 0, 359 );
    windDirection_->setMinimumSize( 50, 50 );
    windDirection_->setMaximumSize( 100, 100 );

    // Temperature
    QLabel* temperatureLabel = new QLabel( tools::translate( "gui::WeatherWidget", "Temperature:" ) );
    temperature_ = new RichSpinBox( "temperature", this, -20, 40, 1 );
    temperature_->setSuffix( tools::translate( "gui::WeatherWidget", "°C" ) );

    // Clouds
    QLabel* cloudsLabel = new QLabel( tools::translate( "gui::WeatherWidget", "Clouds floor/ceiling/density:" ) );
    cloudFloor_   = new RichSpinBox( "cloudFloor", 0, 0, 100000, 100 );
    cloudFloor_->setSuffix( Units::meters.AsString() );
    cloudCeiling_ = new RichSpinBox( "cloudCeiling", 0, 0, 100000, 100 );
    cloudCeiling_->setSuffix( Units::meters.AsString() );
    cloudDensity_ = new RichSpinBox( "cloudDensity", 0, 0, 10, 1 );

    // Type
    QLabel* weatherTypeLabel = new QLabel( tools::translate( "gui::WeatherWidget", "Weather type:" ) );
    type_ = new gui::ValuedComboBox< E_WeatherType >( "type" );
    assert( eNbrWeatherType > 0 );
    for( int i = 0; i < eNbrWeatherType; ++i )
        type_->AddItem( tools::ToDisplayedString( static_cast< E_WeatherType >( i ) ), static_cast< E_WeatherType >( i ) );

    QGridLayout* mainLayout = new QGridLayout( this );
    mainLayout->addWidget( windSpeedLabel, 0, 0 );
    mainLayout->addWidget( windSpeed_, 0, 1, 1, 3 );
    mainLayout->addWidget( windDirectionLabel, 1, 0 );
    mainLayout->addWidget( windDirection_, 1, 1, 1, 3 );
    mainLayout->addWidget( temperatureLabel, 2, 0 );
    mainLayout->addWidget( temperature_, 2, 1, 1, 3 );
    mainLayout->addWidget( cloudsLabel, 3, 0 );
    mainLayout->addWidget( cloudFloor_, 3, 1 );
    mainLayout->addWidget( cloudCeiling_, 3, 2 );
    mainLayout->addWidget( cloudDensity_, 3, 3 );
    mainLayout->addWidget( weatherTypeLabel, 4, 0 );
    mainLayout->addWidget( type_, 4, 1, 1, 3 );
    mainLayout->setAlignment( Qt::AlignTop | Qt::AlignCenter );

    parent->layout()->addWidget( this );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget destructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
WeatherWidget::~WeatherWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::Clear
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherWidget::Clear()
{
    windSpeed_->setValue( 0 );
    windDirection_->setValue( 0 );
    temperature_->setValue( 0 );
    cloudFloor_->setValue( 0 );
    cloudCeiling_->setValue( 0 );
    cloudDensity_->setValue( 0 );
    type_->setCurrentItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::Update
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherWidget::Update( const weather::Meteo& meteo )
{
    windSpeed_->setValue( static_cast< int >( meteo.GetWind().rSpeed_ / meteo.GetConversionFactor() ) );
    windDirection_->setValue( static_cast< int >( meteo.GetWind().eAngle_ ) );
    temperature_->setValue( meteo.GetTemperature() );
    cloudDensity_->setValue( meteo.GetCloud().nDensityPercentage_ );
    cloudFloor_->setValue( meteo.GetCloud().nFloor_ );
    cloudCeiling_->setValue( meteo.GetCloud().nCeiling_ );
    E_WeatherType type = meteo.GetPrecipitation().GetID();
    assert( type >= eWeatherType_None && type < eNbrWeatherType );
    type_->setCurrentItem( static_cast< int >( type ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::CommitTo
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherWidget::CommitTo( weather::Meteo& meteo ) const
{
    weather::WindData wind;
    wind.rSpeed_ = windSpeed_->value() * meteo.GetConversionFactor();
    wind.eAngle_ = windDirection_->value();
    weather::Meteo::sCloudData cloud;
    cloud.nFloor_ = cloudFloor_->value();
    cloud.nCeiling_ = cloudCeiling_->value();
    cloud.nDensityPercentage_ = cloudDensity_->value();
    cloud.rDensity_ = cloudDensity_->value() / 100.;

    meteo.SetWind( wind );
    meteo.SetCloud( cloud );
    meteo.SetTemperature( temperature_->value() );
    const weather::PHY_Precipitation* precipitation = weather::PHY_Precipitation::FindPrecipitation( tools::ToString( type_->GetValue() ).toStdString() );
    if( !precipitation )
        throw MASA_EXCEPTION( "Unknown precipitation: " + tools::ToString( type_->GetValue() ).toStdString() );
    meteo.SetPrecipitation( *precipitation );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::CreateParameters
// Created: JSR 2010-04-23
// -----------------------------------------------------------------------------
gui::WeatherParameters WeatherWidget::CreateParameters()
{
    gui::WeatherParameters result = 
           { type_->GetValue(),
             static_cast< float >( temperature_->value() ),
             static_cast< float >( windSpeed_->value() ),
             windDirection_->value(),
             static_cast< float >( cloudFloor_->value() ),
             static_cast< float >( cloudCeiling_->value() ),
             static_cast< float >( cloudDensity_->value() ) };
    return result;
}
