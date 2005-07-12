//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Illumination_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:51 $
// $Revision: 4 $
// $Workfile: ADN_Sensors_Illumination_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Sensors_Illumination_GUI_h_
#define __ADN_Sensors_Illumination_GUI_h_

#include "ADN_CommonGfx.h"

//*****************************************************************************
// Created: JDY 03-07-08
//*****************************************************************************
class ADN_Sensors_Illumination_GUI
: public ADN_Table2
{
public:
    explicit ADN_Sensors_Illumination_GUI(QWidget * parent = 0 );
    virtual ~ADN_Sensors_Illumination_GUI();
};


#endif // __ADN_Sensors_Illumination_GUI_h_