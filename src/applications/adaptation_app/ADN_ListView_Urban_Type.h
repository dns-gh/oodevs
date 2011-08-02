//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Categories_Size.h $
// $Author: Ape $
// $Modtime: 1/03/05 11:23 $
// $Revision: 3 $
// $Workfile: ADN_ListView_Categories_Size.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_Urban_Type_h_
#define __ADN_ListView_Urban_Type_h_

#include "ADN_ListView.h"


// =============================================================================
/** @class  ADN_ListView_Urban_Type
@brief  ADN_ListView_Urban_Type
*/
// Created: SLG 2010-03-08
// =============================================================================
class ADN_ListView_Urban_Type
    : public ADN_ListView
{
public:
    explicit ADN_ListView_Urban_Type( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_Urban_Type();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
};


#endif // __ADN_ListView_Urban_Type_h_