//*****************************************************************************
//
// $Created: JDY 03-07-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_Composantes_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:59 $
// $Revision: 5 $
// $Workfile: ADN_Units_Composantes_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Units_Composantes_GUI_h_
#define __ADN_Units_Composantes_GUI_h_

#include "ADN_Table.h"
#include "ADN_Equipments_Data.h"

class ADN_Connector_ABC;

//*****************************************************************************
// Created: JDY 03-07-28
//*****************************************************************************
class ADN_Units_Composantes_GUI : public ADN_Table
{
    Q_OBJECT

public:
    explicit ADN_Units_Composantes_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Units_Composantes_GUI();

    void AddRow( int row, void* data );
    virtual void OnContextMenu( const QPoint& pt );
};

#endif // __ADN_Units_Composantes_GUI_h_