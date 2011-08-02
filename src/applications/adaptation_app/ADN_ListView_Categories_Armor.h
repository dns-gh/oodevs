//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Categories_Armor.h $
// $Author: Ape $
// $Modtime: 1/03/05 11:23 $
// $Revision: 3 $
// $Workfile: ADN_ListView_Categories_Armor.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_Categories_Armor_h_
#define __ADN_ListView_Categories_Armor_h_

#include "ADN_ListView.h"


// =============================================================================
/** @class  ADN_ListView_Categories_Armor
    @brief  ADN_ListView_Categories_Armor
*/
// Created: AGN 2004-05-06
// =============================================================================
class ADN_ListView_Categories_Armor
: public ADN_ListView
{
public:
    explicit ADN_ListView_Categories_Armor( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_Categories_Armor();
    void CreateDefaultAttritionHumanEffect();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
};


#endif // __ADN_ListView_Categories_Armor_h_