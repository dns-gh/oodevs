// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AggregatedPosition_ABC_h_
#define __AggregatedPosition_ABC_h_

#include <boost/noncopyable.hpp>

class DtEntityIdentifier;
class DtVector;

namespace plugins
{
namespace vrforces
{
// =============================================================================
/** @class  AggregatedPosition_ABC
    @brief  Aggregated position base class
*/
// Created: SBO 2011-04-04
// =============================================================================
class AggregatedPosition_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             AggregatedPosition_ABC() {}
    virtual ~AggregatedPosition_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Update( const DtEntityIdentifier& identifier, const DtVector& position ) = 0;
    virtual void Clear() = 0;
    //@}
};
}
}

#endif // __AggregatedPosition_ABC_h_
