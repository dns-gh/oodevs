//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_Sensors_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:55 $
// $Revision: 4 $
// $Workfile: ADN_Composantes_Sensors_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Composantes_Sensors_GUI_h_
#define __ADN_Composantes_Sensors_GUI_h_

#include "ADN_CommonGfx.h"

#include <QtGui/qapplication.h>

//*****************************************************************************
// Created: JDY 03-07-22
//*****************************************************************************
class ADN_Composantes_Sensors_GUI
    : public ADN_Table2
{
public:
    explicit ADN_Composantes_Sensors_GUI(QWidget* parent = 0 );
    virtual ~ADN_Composantes_Sensors_GUI();

    virtual void OnContextMenu( int row, int col, const QPoint& pt );

    void CreateNewSensor( int nSensor );
    void RemoveCurrentSensor();
};


#endif // __ADN_Composantes_Sensors_GUI_h_