// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AggregatedState_h_
#define __AggregatedState_h_

#include "AggregatedState_ABC.h"

namespace plugins
{
namespace vrforces
{
    class Agent;
// =============================================================================
/** @class  AggregatedState
    @brief  AggregatedState
*/
// Created: SBO 2011-04-11
// =============================================================================
class AggregatedState : public AggregatedState_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AggregatedState( const Agent& agent );
    virtual ~AggregatedState();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const DtEntityIdentifier& identifier, const DtDamageState& state );
    //@}

private:
    //! @name Member data
    //@{
    const Agent& agent_;
    std::map< const DtEntityIdentifier, DtDamageState > states_;
    //@}
};
}
}

#endif // __AggregatedState_h_
