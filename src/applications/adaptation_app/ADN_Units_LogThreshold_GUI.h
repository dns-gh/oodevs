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

#ifndef __ADN_Units_LogThreshold_GUI_h_
#define __ADN_Units_LogThreshold_GUI_h_

#include "ADN_CommonGfx.h"
#include "ADN_Units_Data.h"

// =============================================================================
/** @class  ADN_Units_LogThreshold_GUI
    @brief  ADN_Units_LogThreshold_GUI
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Units_LogThreshold_GUI
: public ADN_Table2
{
public:
    explicit ADN_Units_LogThreshold_GUI( QWidget* pParent = 0 );
    virtual ~ADN_Units_LogThreshold_GUI();

private:
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    void AddNewLogSupplyClass( helpers::LogisticSupplyClass& data );
    void RemoveCurrentLogSupplyClass();
};


#endif // __ADN_Units_LogThreshold_GUI_h_