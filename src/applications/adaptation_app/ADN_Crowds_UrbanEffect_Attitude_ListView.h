// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Crowds_UrbanEffect_Attitude_ListView_h_
#define __ADN_Crowds_UrbanEffect_Attitude_ListView_h_

#include "ADN_ListView.h"

// =============================================================================
// Created: MMC 2011-03-30
// =============================================================================
class ADN_Crowds_UrbanEffect_Attitude_ListView : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Crowds_UrbanEffect_Attitude_ListView( QWidget* pParent );
    virtual ~ADN_Crowds_UrbanEffect_Attitude_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    //@}
};

#endif // __ADN_Crowds_UrbanEffect_Attitude_ListView_h_
