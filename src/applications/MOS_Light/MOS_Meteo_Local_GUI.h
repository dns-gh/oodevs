//*****************************************************************************
//
// $Created: AGN 03-08-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_Local_GUI.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_Local_GUI.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_Local_GUI_h_
#define __MOS_Meteo_Local_GUI_h_

#include "MOS_Meteo_Global_GUI.h"

class MOS_Meteo_PointListView;

//*****************************************************************************
// Created: AGN 03-08-07
//*****************************************************************************
class MOS_Meteo_Local_GUI
: public MOS_Meteo_Global_GUI
{
public:
    explicit MOS_Meteo_Local_GUI( QWidget* pParent );
    virtual ~MOS_Meteo_Local_GUI();

    virtual void Initialize();
    virtual void SetWeather( MOS_Meteo_ABC* pWeather );

protected:
    virtual void SlotUpdate();
    virtual void SlotCancel();
    
private:
    QLabel* pTitle_;
    MOS_Meteo_PointListView* pPointView_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_Meteo_Local_GUI.inl"
#endif


#endif // __MOS_Meteo_Local_GUI_h_