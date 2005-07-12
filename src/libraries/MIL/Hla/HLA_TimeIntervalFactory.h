// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-10-13 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_TimeIntervalFactory.h $
// $Author: Age $
// $Modtime: 7/12/04 14:40 $
// $Revision: 2 $
// $Workfile: HLA_TimeIntervalFactory.h $
//
// *****************************************************************************

#ifndef __TimeIntervalFactory_h_
#define __TimeIntervalFactory_h_

#include "hla/TimeIntervalFactory_ABC.h"

// =============================================================================
/** @class  HLA_TimeIntervalFactory
    @brief  Time interval factory implementation
*/
// Created: AGE 2004-10-13
// =============================================================================
class HLA_TimeIntervalFactory : public TimeIntervalFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_TimeIntervalFactory();
    virtual ~HLA_TimeIntervalFactory();
    //@}

    //! @name Operations
    //@{
    virtual TimeInterval_ABC& CreateEpsilon();
    virtual TimeInterval_ABC* Decode( Deserializer& deserializer );
    //@}
};

#endif // __TimeIntervalFactory_h_
