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
             ADN_ListView_RoofShapes( QWidget* pParent );
    virtual ~ADN_ListView_RoofShapes();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& point );
    virtual std::string GetToolTipFor( const QModelIndex& item );
    //@}
};

#endif // __ADN_ListView_RoofShapes_h_
