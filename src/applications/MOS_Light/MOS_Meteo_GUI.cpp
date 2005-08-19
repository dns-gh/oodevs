//*****************************************************************************
//
// $Created: AGN 03-08-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_GUI.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_GUI.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_Meteo_GUI.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo_GUI.inl"
#endif

#include "MOS_Meteo_Global_GUI.h"
#include "MOS_Meteo_ListView.h"

#include <qwidgetstack.h>

//-----------------------------------------------------------------------------
// Name: MOS_Meteo_GUI constructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_GUI::MOS_Meteo_GUI( QWidget* pParent )
: QFrame( pParent )
, pGlobalWeatherGUI_( 0 )
{
    QBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setMargin( 20 );
    pLayout->setSpacing( 20 );
    pLayout->setAutoAdd( true );

    pGlobalWeatherGUI_ = new MOS_Meteo_Global_GUI( this );
    pGlobalWeatherGUI_->Initialize();

    new MOS_Meteo_ListView( this );
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_GUI destructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_GUI::~MOS_Meteo_GUI()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_GUI::SetGlobalWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_GUI::SetGlobalWeather( MOS_Meteo_ABC* pGlobalWeather )
{
    assert( pGlobalWeatherGUI_ != 0 );
    pGlobalWeatherGUI_->SetWeather( pGlobalWeather );
}

