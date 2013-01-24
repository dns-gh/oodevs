// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __rpr_BreachableSegmentStruct_h_
#define __rpr_BreachableSegmentStruct_h_

#include "Enums.h"

#include <cstdint>

namespace rpr
{

class LinearSegmentStruct
{
public:
    LinearSegmentStruct();
    virtual ~LinearSegmentStruct();
    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive&  ) const
    {
        //archive << segmentNumber_;
    }
    template< typename Archive >
    void Deserialize( Archive&  )
    {
        //archive >> segmentNumber_;
    }
    //@}
public:
    uint32_t segmentNumber_;
};
class BreachableSegmentStruct
{
public:
    //! @name Constructors/Destructor
    //@{
    BreachableSegmentStruct();
    virtual ~BreachableSegmentStruct();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive&  ) const
    {
        /*int8_t breached = static_cast< int8_t >( breachedState_ );
        archive << segmentParameters_
                << breachedLength_
                << breached;*/
    }
    template< typename Archive >
    void Deserialize( Archive&  )
    {
        /*int8_t breached;
        archive >> segmentParameters_
                >> breachedLength_
                >> breached;
        breachedState_ = static_cast< BreachedStatusEnum8 >( breached );*/
    }
    //@}

public:
    LinearSegmentStruct segmentParameters_;
    uint32_t breachedLength_;
    BreachedStatusEnum8 breachedState_;
    BreachedStatusEnum8 segmentBreached_[8];
};

} // namespace rpr
#endif // __rpr_BreachableSegmentStruct_h_
