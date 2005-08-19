//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_Global_GUI.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_Meteo_Global_GUI.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_Meteo_Global_GUI.h"
#include "moc_MOS_Meteo_Global_GUI.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo_Global_GUI.inl"
#endif

#include "MOS_Meteo_ABC.h"

#include <qgrid.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qdial.h>
#include <qvgroupbox.h>
#include <qpushbutton.h>

//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Global_GUI constructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_Global_GUI::MOS_Meteo_Global_GUI( QWidget* pParentWidget )
: QFrame( pParentWidget )
, pWeather_( 0 )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Global_GUI destructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_Global_GUI::~MOS_Meteo_Global_GUI()
{
    
}



//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Global_GUI::Initialize
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Global_GUI::Initialize()
{
    // create frame - top widget
    this->setFrameStyle( QFrame::Panel | QFrame::Raised );
    this->setLineWidth( 1 );
    
    QBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setMargin( 20 );
    pLayout->setSpacing( 20 );
    pLayout->setAutoAdd( true );

    // Title
    QLabel* pLabel = new QLabel( "Météo Globale", this );
    pLabel->setAlignment( AlignHCenter );


    InitializeWidgets();
    InitializeButtons();
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Global_GUI::UpdateGUI
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Global_GUI::UpdateGUI()
{
    pTemperatureGUI_->setValue( pWeather_->GetTemperature() );

    pWindSpeedGUI_->setValue( pWeather_->GetWindSpeed() );
    if( pWeather_->GetWindDirection() <= 180 )
        pWindDirectionGUI_->setValue( pWeather_->GetWindDirection() + 180 );
    else
        pWindDirectionGUI_->setValue( pWeather_->GetWindDirection() - 180 );

    pCloudFloorGUI_->setValue( pWeather_->GetCloudCoverFloor() );
    pCloudCeilingGUI_->setValue( pWeather_->GetCloudCoverCeiling() );
    pCloudDensityGUI_->setValue( pWeather_->GetCloudDensity() );

    pPrecipitationGUI_->setCurrentItem( pWeather_->GetWeatherType() );
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Global_GUI::SlotUpdate
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Global_GUI::SlotUpdate()
{
    assert( pWeather_ != 0 );
    // Update the global weather and send the message

    pWeather_->SetTemperature( pTemperatureGUI_->value() );

    pWeather_->SetWindSpeed( pWindSpeedGUI_->value() );
    if( pWindDirectionGUI_->value() > 180 )
        pWeather_->SetWindDirection( pWindDirectionGUI_->value() - 180 );
    else
        pWeather_->SetWindDirection( pWindDirectionGUI_->value() + 180 );

    pWeather_->SetCloudCoverFloor( pCloudFloorGUI_->value() );
    pWeather_->SetCloudCoverCeiling( pCloudCeilingGUI_->value() );
    pWeather_->SetCloudDensity( pCloudDensityGUI_->value() );

    pWeather_->SetWeatherType( (MOS_Meteo_ABC::E_WeatherType)(pPrecipitationGUI_->currentItem()) );

    pWeather_->SendUpdateMsg();
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Global_GUI::SlotCancel
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Global_GUI::SlotCancel()
{
    assert( pWeather_ != 0 );
    // Cancel the modifications on the display
    UpdateGUI();
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Global_GUI::InitializeWidgets
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Global_GUI::InitializeWidgets()
{
    //////////////
    // Temperature
    QGrid* pGrid =new QGrid( 2,this );

    QLabel* pLabel = new QLabel( "Température", pGrid );
    pTemperatureGUI_ = new QSpinBox( 0, 200, 1, pGrid );

    //////////////
    // Wind

    QVGroupBox* pGroup = new QVGroupBox( "Vent", this );
    pGrid =new QGrid( 2,pGroup );

    // Speed
    pLabel = new QLabel( "Vitesse", pGrid );
    pWindSpeedGUI_ = new QSpinBox( 0, 200, 1, pGrid );

    // Direction
    pLabel = new QLabel( "Direction", pGrid );
    pWindDirectionGUI_ = new QDial( 0, 360, 1, 180, pGrid );
    pWindDirectionGUI_->setWrapping( true );
    pWindDirectionGUI_->setNotchesVisible( true );
    pWindDirectionGUI_->setNotchTarget( 360.0 / 4.0 );

    //////////////
    // Cloud
    pGroup = new QVGroupBox( "Couverture nuageuse", this );
    pGrid =new QGrid( 2,pGroup );

    // Floor
    pLabel = new QLabel( "Plancher", pGrid );
    pCloudFloorGUI_ = new QSpinBox( 0, 8000, 1, pGrid );

    // Ceiling
    pLabel = new QLabel( "Plafond", pGrid );
    pCloudCeilingGUI_ = new QSpinBox( 0, 8000, 1, pGrid );

    // Density
    pLabel = new QLabel( "Densité", pGrid );
    pCloudDensityGUI_ = new QSpinBox( 0, 100, 1, pGrid );

    //////////////
    // Precipitation
    pGrid =new QGrid( 2,this );
    pLabel = new QLabel( "Précipitation", pGrid );
    pPrecipitationGUI_ = new QComboBox( pGrid );
    pPrecipitationGUI_->insertItem( "Beau", MOS_Meteo_ABC::eWeather_Clear );
    pPrecipitationGUI_->insertItem( "Tempête", MOS_Meteo_ABC::eWeather_Storm );
    pPrecipitationGUI_->insertItem( "Brouillard", MOS_Meteo_ABC::eWeather_Fog );
    pPrecipitationGUI_->insertItem( "Crachin", MOS_Meteo_ABC::eWeather_Drizzle );
    pPrecipitationGUI_->insertItem( "Pluie", MOS_Meteo_ABC::eWeather_Rain );
    pPrecipitationGUI_->insertItem( "Neige", MOS_Meteo_ABC::eWeather_Snow );    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Global_GUI::InitializeButtons
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Global_GUI::InitializeButtons()
{
    ///////////////
    // Buttons
    QGrid* pGrid =new QGrid( 2,this );
    QPushButton* pButton = new QPushButton( "Envoyer", pGrid );
    connect( pButton, SIGNAL( clicked() ), SLOT( SlotUpdate() ) );
    pButton = new QPushButton( "Annuler", pGrid );
    connect( pButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );    
}

