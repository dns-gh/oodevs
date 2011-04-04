// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AggregatedPosition_h_
#define __AggregatedPosition_h_

#include "AggregatedPosition_ABC.h"
#include "geometry/Types.h"
#include <vlpi/entityidentifier.h>

namespace plugins
{
namespace vrforces
{
    class Agent;

// =============================================================================
/** @class  AggregatedPosition
    @brief  AggregatedPosition
*/
// Created: SBO 2011-04-04
// =============================================================================
class AggregatedPosition : public AggregatedPosition_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AggregatedPosition( const Agent& agent );
    virtual ~AggregatedPosition();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const DtEntityIdentifier& identifier, const DtVector& position );
    virtual void Clear();
    //@}

private:
    //! @name Helpers
    //@{
    void ComputeBarycenter();
    //@}

private:
    //! @name Member data
    //@{
    const Agent& agent_;
    std::map< const DtEntityIdentifier, geometry::Point2d > points_;
    geometry::Point2d barycenter_;
    geometry::Point2d last_;
    //@}
};
}
}

#endif // __AggregatedPosition_h_
