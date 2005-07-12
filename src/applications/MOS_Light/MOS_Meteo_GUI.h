//*****************************************************************************
//
// $Created: AGN 03-08-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_GUI.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_GUI.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_GUI_h_
#define __MOS_Meteo_GUI_h_

#include <qframe.h>

class MOS_Meteo_Global_GUI;
class MOS_Meteo_ABC;
class QWidgetStack;

//*****************************************************************************
// Created: AGN 03-08-07
//*****************************************************************************
class MOS_Meteo_GUI
: public QFrame
{
public:
    explicit MOS_Meteo_GUI( QWidget* pParent );
    virtual ~MOS_Meteo_GUI();

    void SetGlobalWeather( MOS_Meteo_ABC* pGlobalWeather );

private:
    MOS_Meteo_Global_GUI* pGlobalWeatherGUI_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_Meteo_GUI.inl"
#endif


#endif // __MOS_Meteo_GUI_h_