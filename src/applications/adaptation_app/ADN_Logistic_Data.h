// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Logistic_Data_h_
#define __ADN_Logistic_Data_h_

#include <boost/noncopyable.hpp>
#include "ADN_Data_Container.h"

// =============================================================================
/** @class  ADN_Logistic_Data
    @brief  ADN_Logistic_Data
*/
// Created: ABR 2012-01-18
// =============================================================================
class ADN_Logistic_Data : public ADN_Data_Container
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Logistic_Data();
    virtual ~ADN_Logistic_Data();
    //@}
};

#endif // __ADN_Logistic_Data_h_
