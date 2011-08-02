// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Population_FireEffect_Attitude_ListView_h_
#define __ADN_Population_FireEffect_Attitude_ListView_h_

#include "ADN_ListView.h"
#include <boost/noncopyable.hpp>

// =============================================================================
// Created: SBO 2005-10-25
// =============================================================================
class ADN_Population_FireEffect_Attitude_ListView : public ADN_ListView
                                                  , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Population_FireEffect_Attitude_ListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_Population_FireEffect_Attitude_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    //@}
};

#endif // __ADN_Population_FireEffect_Attitude_ListView_h_
