// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AggregatedState_ABC_h_
#define __AggregatedState_ABC_h_

#include <boost/noncopyable.hpp>
#include "vlpi/disEnums.h"

class DtEntityIdentifier;

namespace plugins
{
namespace vrforces
{
// =============================================================================
/** @class  AggregatedState_ABC
    @brief  AggregatedState_ABC
*/
// Created: SBO 2011-04-11
// =============================================================================
class AggregatedState_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             AggregatedState_ABC() {}
    virtual ~AggregatedState_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Update( const DtEntityIdentifier& identifier, const DtDamageState& state ) = 0;
    //@}
};
}
}

#endif // __AggregatedState_ABC_h_
