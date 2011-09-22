// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ListView_RoofShapes_h_
#define __ADN_ListView_RoofShapes_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_RoofShapes
    @brief  ADN list view roof shapes
*/
// Created: LGY 2011-09-21
// =============================================================================
class ADN_ListView_RoofShapes : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_RoofShapes( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_RoofShapes();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& point );
    //@}
};

#endif // __ADN_ListView_RoofShapes_h_
