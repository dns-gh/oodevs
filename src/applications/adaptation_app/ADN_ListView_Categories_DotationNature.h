// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ListView_Categories_DotationNature_h_
#define __ADN_ListView_Categories_DotationNature_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_Categories_DotationNature
    @brief  ADN_ListView_Categories_DotationNature
*/
// Created: SBO 2006-03-23
// =============================================================================
class ADN_ListView_Categories_DotationNature : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_Categories_DotationNature( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_Categories_DotationNature();
    //@}

private:
    //! @name Helpers
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_ListView_Categories_DotationNature_h_
