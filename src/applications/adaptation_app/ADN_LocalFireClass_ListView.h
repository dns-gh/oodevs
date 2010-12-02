// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_LocalFireClass_ListView_h_
#define __ADN_LocalFireClass_ListView_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_LocalFireClass_ListView
    @brief  ADN_LocalFireClass_ListView
*/
// Created: JSR 2010-12-02
// =============================================================================
class ADN_LocalFireClass_ListView : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_LocalFireClass_ListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    virtual ~ADN_LocalFireClass_ListView();
    //@}

    //! @name Operations
    //@{
    virtual void ConnectItem( bool bConnect );
    virtual void OnContextMenu( const QPoint& pt );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ADN_LocalFireClass_ListView_h_
