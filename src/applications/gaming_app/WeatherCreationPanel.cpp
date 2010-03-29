// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeatherCreationPanel.h"
#include "moc_WeatherCreationPanel.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_gui/ParametersLayer.h"
#include "gaming/StaticModel.h"
#include "protocol/Publisher_ABC.h"
#include "protocol/SimulationSenders.h"
#include "WeatherWidget.h"

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel constructor
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
WeatherCreationPanel::WeatherCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, Publisher_ABC& publisher, const StaticModel& model, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools )
    : gui::InfoPanel_ABC( parent, panel, tr( "Weathers" ), "WeatherCreationPanel" )
    , controllers_      ( controllers )
    , layer_            ( layer )
    , publisher_        ( publisher )
    , tools_            ( tools )
    , location_         ( 0 )
    , serializer_       ( model.coordinateConverter_ )
{
    
    QGroupBox* localGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Weather" ), this );
    QVBox* localWeatherBox = new QVBox( localGroup );
    localWeather_ = new WeatherWidget( localWeatherBox, tr( "Weather parameters" ) );

    QGroupBox* parametersGroup = new QGroupBox( 2, Qt::Horizontal, tr( "Time and Position Parameters" ), localWeatherBox );
    new QLabel( tr( "Start time:" ), parametersGroup );
    startTime_ = new QDateTimeEdit( parametersGroup );
    new QLabel( tr( "End time:" ), parametersGroup );
    endTime_ = new QDateTimeEdit( parametersGroup );

    QCheckBox* weatherType = new QCheckBox( tr( "Set as global weather" ), parametersGroup );
    weatherType->setChecked( false );
    
    positionBtn_ = new QPushButton( tr( "Set location" ), parametersGroup );

    QHBox* box = new QHBox( this );
    box->setMaximumHeight( 30 );
    QButton* okBtn     = new QPushButton( tr( "Save" )  , box );
    QButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );

    connect( positionBtn_, SIGNAL( clicked() ), this, SLOT( StartEditingLocation() ) );
    connect( okBtn, SIGNAL( clicked() ), this, SLOT( Commit() ) );
    connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( Reset() ) );
    connect( weatherType, SIGNAL( toggled( bool ) ), this, SLOT( OnToogleWeatherType( bool ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel destructor
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
WeatherCreationPanel::~WeatherCreationPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::NotifyUpdated
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void WeatherCreationPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::StartEditingLocation
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void WeatherCreationPanel::StartEditingLocation()
{
    ResetLocation();
    layer_.StartRectangle( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::CheckValidity
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
bool WeatherCreationPanel::CheckValidity()
{
    return ( ( startTime_->dateTime() < endTime_->dateTime() ) && location_ );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::Commit
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void WeatherCreationPanel::Commit()
{
    if( CheckValidity() )
    {
        simulation::ControlLocalMeteo message;
        Common::MsgMeteoAttributes* att = message().mutable_attributes();
        localWeather_->Commit( *att );

        Common::MsgLocation longlat;
        serializer_.Serialize( *location_, longlat );
        message().mutable_start_time()->set_data( startTime_->dateTime().toString( "yyyyMMddThhmmss" ).ascii() );
        message().mutable_end_time()->set_data( endTime_->dateTime().toString( "yyyyMMddThhmmss" ).ascii() );
        message().mutable_bottom_right_coordinate()->set_latitude( longlat.coordinates().elem( 0 ).latitude()  );
        message().mutable_bottom_right_coordinate()->set_longitude( longlat.coordinates().elem( 0 ).longitude()  );
        message().mutable_top_left_coordinate()->set_latitude( longlat.coordinates().elem( 1 ).latitude()  );
        message().mutable_top_left_coordinate()->set_longitude( longlat.coordinates().elem( 1 ).longitude()  );
        message.Send( publisher_ );
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::OnToogleWeatherType
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::OnToogleWeatherType( bool value )
{
    ResetLocation();
    if( value )
    {
        ResetLocation();
        layer_.SelectRaster( *this );
        positionBtn_->setEnabled( false );
    }
    else
        positionBtn_->setEnabled( true );
}
// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::Reset
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void WeatherCreationPanel::Reset()
{
    ResetLocation();
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::Handle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::Handle( kernel::Location_ABC& location )
{
    if( location.IsValid() )
        location_ = &location;
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::Draw
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::Draw( const kernel::Viewport_ABC& /*viewport*/ )
{
    if( location_ && bLocationDisplay_ )
        ShapeHandler_ABC::Draw( *location_, geometry::Rectangle2f(), tools_ );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::showEvent
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::showEvent( QShowEvent* )
{
    bLocationDisplay_ = true;
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::hideEvent
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::hideEvent( QHideEvent* )
{
    bLocationDisplay_ = false;
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::ResetLocation
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::ResetLocation()
{
    delete location_; location_ = 0;
}