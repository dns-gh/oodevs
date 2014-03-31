// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
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
#include <map>

// =============================================================================
/** @class  ADN_ListView_Orders
    @brief  ADN_ListView_Orders
*/
// Created: AGN 2003-11-27
// =============================================================================
class ADN_ListView_Orders : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_Orders( QWidget* parent );
    virtual ~ADN_ListView_Orders();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( const QPoint& pt );
    bool Contains( const std::string& strComposanteName ) const;

    void CreateNewItem( int n );
    void RemoveCurrentItem();
    //@}

private:
    //! @name Helpers
    //@{
    void ConnectItem( bool bConnect );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< int, std::string > T_FragOrders;
    //@}

private:
    //! @name data
    //@{
    T_FragOrders fragOrders_;
    //@}
};

#endif // __ADN_ListView_Orders_h_
