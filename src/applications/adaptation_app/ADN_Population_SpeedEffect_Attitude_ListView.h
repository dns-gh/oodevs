// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Population_SpeedEffect_Attitude_ListView_h_
#define __ADN_Population_SpeedEffect_Attitude_ListView_h_

#include "ADN_ListView.h"
#include <boost/noncopyable.hpp>

// =============================================================================
// Created: SBO 2005-10-25
// =============================================================================
class ADN_Population_SpeedEffect_Attitude_ListView : public ADN_ListView
                                                   , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Population_SpeedEffect_Attitude_ListView( QWidget* pParent );
    virtual ~ADN_Population_SpeedEffect_Attitude_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    //@}
};

#endif // __ADN_Population_SpeedEffect_Attitude_ListView_h_
