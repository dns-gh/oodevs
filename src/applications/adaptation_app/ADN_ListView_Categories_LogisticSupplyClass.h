// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ListView_Categories_LogisticSupplyClass_h_
#define __ADN_ListView_Categories_LogisticSupplyClass_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_Categories_LogisticSupplyClass
    @brief  ADN_ListView_Categories_LogisticSupplyClass
*/
// Created: SBO 2006-03-23
// =============================================================================
class ADN_ListView_Categories_LogisticSupplyClass : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_Categories_LogisticSupplyClass( QWidget* pParent );
    virtual ~ADN_ListView_Categories_LogisticSupplyClass();
    //@}

private:
    //! @name Helpers
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    virtual std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_ListView_Categories_LogisticSupplyClass_h_
