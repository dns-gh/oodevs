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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_TimeFactory.h $
// $Author: Age $
// $Modtime: 5/11/04 11:04 $
// $Revision: 1 $
// $Workfile: HLA_TimeFactory.h $
//
// *****************************************************************************

#ifndef __TimeFactory_h_
#define __TimeFactory_h_

#include <hla/TimeFactory_ABC.h>

// =============================================================================
/** @class  HLA_TimeFactory
    @brief  Time factory implementation
*/
// Created: AGE 2004-10-13
// =============================================================================
class HLA_TimeFactory : public hla::TimeFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_TimeFactory();
    virtual ~HLA_TimeFactory();
    //@}

    //! @name Operations
    //@{
    virtual hla::Time_ABC& CreateInitial();

    virtual hla::Time_ABC* Decode( hla::Deserializer& deserializer );
    //@}
};

#endif // __TimeFactory_h_
