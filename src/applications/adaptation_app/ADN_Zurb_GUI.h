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

#ifndef __ADN_Zurb_GUI_h_
#define __ADN_Zurb_GUI_h_

#include "ADN_GUI_ABC.h"
#include <qobject.h>

class ADN_Zurb_Data;
class ADN_ListView_Zurb_Type;


// =============================================================================
/** @class  ADN_Categories_GUI
@brief  ADN_Categories_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Zurb_GUI 
    : public ADN_GUI_ABC
{
    Q_OBJECT

public:

    enum E_ZurbGuiElements
    {
        eZurbName,
        eNbrZurbGuiElements
    };

public:
             ADN_Zurb_GUI( ADN_Zurb_Data& data );
    virtual ~ADN_Zurb_GUI();

    void Build();

private:
    ADN_Zurb_Data& data_;

    ADN_ListView_Zurb_Type*           pListMaterial_;
    ADN_ListView_Zurb_Type*           pListFacade_;
    ADN_ListView_Zurb_Type*           pListRoofShape_;
};


#endif // __ADN_Zurb_GUI_h_
