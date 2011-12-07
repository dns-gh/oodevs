//*****************************************************************************
//
// $Created: JDY 03-07-23 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Composantes_Objects.h $
// $Author: Ape $
// $Modtime: 1/03/05 18:19 $
// $Revision: 3 $
// $Workfile: ADN_ListView_Composantes_Objects.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_Composantes_Objects_h_
#define __ADN_ListView_Composantes_Objects_h_

#include "ADN_ListView.h"
#include "ADN_Objects_Data.h"


// =============================================================================
/** @class  ADN_ListView_Composantes_Objects
    @brief  ADN_ListView_Composantes_Objects
*/
// Created: APE 2005-01-18
// =============================================================================
class ADN_ListView_Composantes_Objects
: public ADN_ListView
{
    Q_OBJECT

public:
    explicit ADN_ListView_Composantes_Objects(QWidget * parent = 0, const char * name = 0, Qt::WFlags f = 0);
    virtual ~ADN_ListView_Composantes_Objects();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    bool Contains( const ADN_Objects_Data_ObjectInfos* pInfo );

};


#endif // __ADN_ListView_Composantes_Objects_h_