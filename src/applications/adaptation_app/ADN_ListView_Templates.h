// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ListView_Templates_h_
#define __ADN_ListView_Templates_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_Templates
    @brief  ADN list view templates
*/
// Created: LGY 2011-09-20
// =============================================================================
class ADN_ListView_Templates : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_Templates( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_Templates();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& point );
    //@}
};

#endif // __ADN_ListView_Templates_h_
