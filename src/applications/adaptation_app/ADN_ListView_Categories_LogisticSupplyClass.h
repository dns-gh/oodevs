// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
             ADN_ListView_Categories_LogisticSupplyClass( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_Categories_LogisticSupplyClass();
    //@}

private:
    //! @name Helpers
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_ListView_Categories_LogisticSupplyClass_h_
