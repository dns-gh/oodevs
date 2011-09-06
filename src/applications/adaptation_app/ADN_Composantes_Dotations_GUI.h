//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_Dotations_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:56 $
// $Revision: 6 $
// $Workfile: ADN_Composantes_Dotations_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Composantes_Dotations_GUI_h_
#define __ADN_Composantes_Dotations_GUI_h_

#include "ADN_CommonGfx.h"
#include "ADN_Equipement_Data.h"

// =============================================================================
/** @class  ADN_Composantes_Dotations_GUI
    @brief  ADN_Composantes_Dotations_GUI
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Composantes_Dotations_GUI
: public ADN_Table2
{
    Q_OBJECT

public:
    explicit ADN_Composantes_Dotations_GUI( bool bIncludeNormalizedConsumption, QWidget* pParent = 0, bool bIncludeThreshold = true, bool bQtyDecimal = true );
    virtual ~ADN_Composantes_Dotations_GUI();

private:
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    void AddNewDotation( ADN_Equipement_Data::CategoryInfo& category );
    void RemoveCurrentDotation();
    bool Contains( ADN_Equipement_Data::CategoryInfo& category );

public:
    bool bIncludeNormalizedConsumption_;
    bool bIncludeThreshold_;
};


#endif // __ADN_Composantes_Dotations_GUI_h_