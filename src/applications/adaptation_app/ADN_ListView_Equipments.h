// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ListView_Equipments_h_
#define __ADN_ListView_Equipments_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_Equipments
    @brief  ADN_ListView_Equipments
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_ListView_Equipments : public ADN_ListView
{
public:
    explicit ADN_ListView_Equipments( QWidget* pParent );
    virtual ~ADN_ListView_Equipments();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );

    std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_ListView_Equipments_h_