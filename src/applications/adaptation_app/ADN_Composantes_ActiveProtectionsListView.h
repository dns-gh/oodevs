//*****************************************************************************
//
// $Created: FDS 10-02-24 $
// $Workfile: ADN_Composantes_ActiveProtectionsListView.h $
//
//*****************************************************************************

#ifndef __ADN_Composantes_ActiveProtectionsListView_h_
#define __ADN_Composantes_ActiveProtectionsListView_h_

#include "ADN_ListView.h"

#include "ADN_ActiveProtections_Data.h"


// =============================================================================
/** @class  ADN_Composantes_ActiveProtectionsListView
    @brief  ADN_Composantes_ActiveProtectionsListView
*/
// Created: FDS 2010-02-24
// =============================================================================
class ADN_Composantes_ActiveProtectionsListView
: public ADN_ListView
{
    Q_OBJECT

public:
    explicit ADN_Composantes_ActiveProtectionsListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_Composantes_ActiveProtectionsListView();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    bool Contains( const ADN_ActiveProtections_Data::ActiveProtectionsInfos* pInfo );
};


#endif // __ADN_Composantes_ActiveProtectionsListView_h_