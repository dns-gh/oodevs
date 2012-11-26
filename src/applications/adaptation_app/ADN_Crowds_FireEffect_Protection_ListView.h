// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Crowds_FireEffect_Protection_ListView_h_
#define __ADN_Crowds_FireEffect_Protection_ListView_h_

#include "ADN_ListView.h"

// =============================================================================
// Created: SBO 2005-10-25
// =============================================================================
class ADN_Crowds_FireEffect_Protection_ListView : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Crowds_FireEffect_Protection_ListView( QWidget* pParent );
    virtual ~ADN_Crowds_FireEffect_Protection_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    //@}
};

#endif // __ADN_Crowds_FireEffect_Protection_ListView_h_
