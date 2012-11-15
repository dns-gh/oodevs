// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ListView_Disasters_h_
#define __ADN_ListView_Disasters_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_Disasters
    @brief  ADN_ListView_Disasters
*/
// Created: LGY 2012-11-13
// =============================================================================
class ADN_ListView_Disasters : public ADN_ListView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_Disasters( QWidget* pParent );
    virtual ~ADN_ListView_Disasters();
    //@}

private:
    //! @name Operations
    //@{
    virtual void ConnectItem( bool bConnect );
    virtual void OnContextMenu( const QPoint& point);
    virtual std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_ListView_Disasters_h_
