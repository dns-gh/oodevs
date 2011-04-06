// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterFactory_h_
#define __FilterFactory_h_

#include "FilterFactory_ABC.h"

namespace mission_tester
{
// =============================================================================
/** @class  FilterFactory
    @brief  FilterFactory
*/
// Created: PHC 2011-04-06
// =============================================================================
class FilterFactory : public FilterFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FilterFactory();
    virtual ~FilterFactory();
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< Filter_ABC > Create( const std::string& criteria );
    //@}
};
}

#endif // __FilterFactory_h_
