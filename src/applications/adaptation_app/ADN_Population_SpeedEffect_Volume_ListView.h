// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Population_SpeedEffect_Volume_ListView_h_
#define __ADN_Population_SpeedEffect_Volume_ListView_h_

#include "ADN_ListView.h"


// =============================================================================
// Created: SBO 2005-10-25
// =============================================================================
class ADN_Population_SpeedEffect_Volume_ListView
: public ADN_ListView
{
    MT_COPYNOTALLOWED( ADN_Population_SpeedEffect_Volume_ListView )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Population_SpeedEffect_Volume_ListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    virtual ~ADN_Population_SpeedEffect_Volume_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    //@}
};

#endif // __ADN_Population_SpeedEffect_Volume_ListView_h_
