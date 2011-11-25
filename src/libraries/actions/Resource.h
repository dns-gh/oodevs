// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Resource_h_
#define __Resource_h_

#include "String.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Resource
    @brief  Resource
*/
// Created: ABR 2011-11-24
// =============================================================================
class Resource : public String
{

public:
    //! @name Constructors/Destructor
    //@{
             Resource( const kernel::OrderParameter& parameter, const std::string& value );
    virtual ~Resource();
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}
};

}
}

#endif // __Resource_h_
