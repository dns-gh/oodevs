//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Categories_GUI.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:56 $
// $Revision: 8 $
// $Workfile: ADN_Categories_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Urban_GUI_h_
#define __ADN_Urban_GUI_h_

#include "ADN_GUI_ABC.h"
#include <qobject.h>

class ADN_Urban_Data;
class ADN_ListView_Urban_Type;


// =============================================================================
/** @class  ADN_Categories_GUI
@brief  ADN_Categories_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Urban_GUI 
    : public ADN_GUI_ABC
{
    Q_OBJECT

public:

    enum E_UrbanGuiElements
    {
        eUrbanName,
        eNbrUrbanGuiElements
    };

public:
             ADN_Urban_GUI( ADN_Urban_Data& data );
    virtual ~ADN_Urban_GUI();

    void Build();

private:
    ADN_Urban_Data& data_;

    ADN_ListView_Urban_Type*           pListMaterial_;
    ADN_ListView_Urban_Type*           pListFacade_;
    ADN_ListView_Urban_Type*           pListRoofShape_;
};


#endif // __ADN_Urban_GUI_h_
