//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_WeaponsListView.h $
// $Author: Ape $
// $Modtime: 2/03/05 15:34 $
// $Revision: 2 $
// $Workfile: ADN_Composantes_WeaponsListView.h $
//
//*****************************************************************************

#ifndef __ADN_Composantes_WeaponsListView_h_
#define __ADN_Composantes_WeaponsListView_h_

#include "ADN_ListView.h"

#include "ADN_Weapons_Data.h"


// =============================================================================
/** @class  ADN_Composantes_WeaponsListView
    @brief  ADN_Composantes_WeaponsListView
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_Composantes_WeaponsListView
: public ADN_ListView
{
    Q_OBJECT

public:
    explicit ADN_Composantes_WeaponsListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_Composantes_WeaponsListView();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    bool Contains( const ADN_Weapons_Data::WeaponInfos* pInfo );
};


#endif // __ADN_Composantes_WeaponsListView_h_