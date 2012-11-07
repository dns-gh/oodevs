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
    : public ADN_Table
{
public:
    explicit ADN_Composantes_Sensors_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Composantes_Sensors_GUI();

    virtual void OnContextMenu( const QPoint& pt );
    virtual void AddRow( int row, void* data );

    void CreateNewSensor( int nSensor );
    void RemoveCurrentSensor();
};

#endif // __ADN_Composantes_Sensors_GUI_h_