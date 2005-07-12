//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_Global_GUI.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_Global_GUI.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_Global_GUI_h_
#define __MOS_Meteo_Global_GUI_h_

#include "MOS_Types.h"
#include <qframe.h>
#include <QObjectDefs.h>

class QSpinBox;
class QComboBox;
class QDial;
class MOS_Meteo_ABC;

//*****************************************************************************
// Created: AGN 03-08-06
//*****************************************************************************
class MOS_Meteo_Global_GUI
: public QFrame
{
    Q_OBJECT
public:
    explicit MOS_Meteo_Global_GUI( QWidget* pParentWidget );
    virtual ~MOS_Meteo_Global_GUI();

    virtual void Initialize();

    virtual void SetWeather( MOS_Meteo_ABC* pWeather );

protected:

    void UpdateGUI();

    void InitializeWidgets();
    void InitializeButtons();

protected slots:
    virtual void SlotUpdate();
    virtual void SlotCancel();
    
protected:
    QSpinBox* pTemperatureGUI_;

    QSpinBox* pWindSpeedGUI_;
    QDial*    pWindDirectionGUI_;
    
    QSpinBox* pCloudFloorGUI_;
    QSpinBox* pCloudCeilingGUI_;
    QSpinBox* pCloudDensityGUI_;

    QComboBox* pPrecipitationGUI_;

    MOS_Meteo_ABC* pWeather_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_Meteo_Global_GUI.inl"
#endif


#endif // __MOS_Meteo_Global_GUI_h_