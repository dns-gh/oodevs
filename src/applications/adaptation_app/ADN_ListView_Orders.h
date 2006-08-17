// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Orders.h $
// $Author: Ape $
// $Modtime: 2/03/05 17:09 $
// $Revision: 3 $
// $Workfile: ADN_ListView_Orders.h $
//
// *****************************************************************************

#ifndef __ADN_ListView_Orders_h_
#define __ADN_ListView_Orders_h_

#include "ADN_Types.h"
#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_Orders
    @brief  ADN_ListView_Orders
*/
// Created: AGN 2003-11-27
// =============================================================================
class ADN_ListView_Orders
: public ADN_ListView
{
public:
    explicit ADN_ListView_Orders(QWidget * parent = 0, const char * name = 0, WFlags f = 0);
    virtual ~ADN_ListView_Orders();

    virtual void OnContextMenu( const QPoint& pt );
    bool Contains( const std::string& strComposanteName ) const;

    void CreateNewItem( int n );
    void RemoveCurrentItem();

private:
    void ConnectItem( bool bConnect );
};


#endif // __ADN_ListView_Orders_h_
