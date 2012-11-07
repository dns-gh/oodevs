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
class ADN_Composantes_Dotations_GUI : public ADN_Table3
{
    Q_OBJECT

public:
             ADN_Composantes_Dotations_GUI( const QString& objectName, ADN_Connector_ABC*& connector,
                                            bool bIncludeNormalizedConsumption, QWidget* pParent = 0, bool bIncludeThreshold = true );
    virtual ~ADN_Composantes_Dotations_GUI();

private:
    virtual void OnContextMenu( const QPoint& pt );
    void AddNewDotation( ADN_Equipement_Data::CategoryInfo& category );
    void RemoveCurrentDotation();
    bool Contains( ADN_Equipement_Data::CategoryInfo& category );
    virtual void AddRow( int row, void* data );

public:
    bool bIncludeNormalizedConsumption_;
    bool bIncludeThreshold_;
};

#endif // __ADN_Composantes_Dotations_GUI_h_