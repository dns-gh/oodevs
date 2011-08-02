// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_FireClass_ListView_h_
#define __ADN_FireClass_ListView_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_FireClass_ListView
    @brief  ADN_FireClass_ListView
*/
// Created: JSR 2010-12-02
// =============================================================================
class ADN_FireClass_ListView : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_FireClass_ListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_FireClass_ListView();
    //@}

    //! @name Operations
    //@{
    virtual void ConnectItem( bool bConnect );
    virtual void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_FireClass_ListView_h_
