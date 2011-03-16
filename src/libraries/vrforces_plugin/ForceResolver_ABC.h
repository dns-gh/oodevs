// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ForceResolver_ABC_h_
#define __ForceResolver_ABC_h_

#include <vlpi/disEnums.h>
#include <boost/noncopyable.hpp>

namespace plugins
{
namespace vrforces
{
// =============================================================================
/** @class  ForceResolver_ABC
    @brief  Force resolver definition
*/
// Created: SBO 2011-03-16
// =============================================================================
class ForceResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ForceResolver_ABC() {}
    virtual ~ForceResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual DtForceType Resolve( unsigned long identifier ) const = 0;
    //@}
};
}
}

#endif // __ForceResolver_ABC_h_
