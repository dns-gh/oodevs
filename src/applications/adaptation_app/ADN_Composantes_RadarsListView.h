//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_RadarsListView.h $
// $Author: Nld $
// $Modtime: 3/05/05 15:21 $
// $Revision: 1 $
// $Workfile: ADN_Composantes_RadarsListView.h $
//
//*****************************************************************************

#ifndef __ADN_Composantes_RadarsListView_h_
#define __ADN_Composantes_RadarsListView_h_

#include "ADN_ListView.h"
#include "ADN_Radars_Data.h"


// =============================================================================
/** @class  ADN_Composantes_RadarsListView
    @brief  ADN_Composantes_RadarsListView
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_Composantes_RadarsListView
    : public ADN_ListView
{
    Q_OBJECT

public:
    explicit ADN_Composantes_RadarsListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_Composantes_RadarsListView();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    bool Contains( const ADN_Radars_Data::RadarInfos* pInfo );
};


#endif // __ADN_Composantes_RadarsListView_h_