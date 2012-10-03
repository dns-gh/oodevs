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

#ifndef __ADN_ListView_UrbanAccommodation_Type_h_
#define __ADN_ListView_UrbanAccommodation_Type_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_UrbanAccommodation_Type
@brief  ADN_ListView_UrbanAccommodation_Type
*/
// Created: SLG 2010-12-20
// =============================================================================
class ADN_ListView_UrbanAccommodation_Type
    : public ADN_ListView
{
public:
    explicit ADN_ListView_UrbanAccommodation_Type( QWidget* pParent );
    virtual ~ADN_ListView_UrbanAccommodation_Type();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    virtual std::string GetToolTipFor( const QModelIndex& index );
};

#endif // __ADN_ListView_UrbanAccommodation_Type_h_
