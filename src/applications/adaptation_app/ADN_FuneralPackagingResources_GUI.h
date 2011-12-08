//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_LogThreshold_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:56 $
// $Revision: 6 $
// $Workfile: ADN_Units_LogThreshold_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_FuneralPackagingResources_GUI_h_
#define __ADN_FuneralPackagingResources_GUI_h_

#include "ADN_CommonGfx.h"
#include "ADN_Equipement_Data.h"

// =============================================================================
/** @class  ADN_FuneralPackagingResources_GUI
    @brief  ADN_FuneralPackagingResources_GUI
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_FuneralPackagingResources_GUI
: public ADN_Table2
{
public:
    explicit ADN_FuneralPackagingResources_GUI( QWidget* pParent = 0 );
    virtual ~ADN_FuneralPackagingResources_GUI();

private:
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    bool Contains( ADN_Equipement_Data::CategoryInfo& category );
    void AddNewDotation( ADN_Equipement_Data::CategoryInfo& category );
    void RemoveCurrentDotation();
};


#endif // __ADN_FuneralPackagingResources_GUI_h_
