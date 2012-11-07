//*****************************************************************************
//
// $Created: JDY 03-07-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_Postures_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:55 $
// $Revision: 4 $
// $Workfile: ADN_Units_Postures_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Units_Postures_GUI_h_
#define __ADN_Units_Postures_GUI_h_

#include "ADN_Table.h"

//*****************************************************************************
// Created: JDY 03-07-28
//*****************************************************************************
class ADN_Units_Postures_GUI : public ADN_Table3
{
public:
             ADN_Units_Postures_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Units_Postures_GUI();

    void AddRow( int row, void* data );
};

#endif // __ADN_Units_Postures_GUI_h_