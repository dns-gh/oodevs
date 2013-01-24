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
#include "Coordinates.h"

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
    void Serialize( Archive& archive ) const
    {
        uint8_t deactivated = deactivated_;
        uint8_t flaming = flaming_;
        uint8_t predistributed = predistributed_;
        uint8_t smoking = smoking_;
        uint32_t damaged = static_cast< uint32_t >( damagedState_ );
        archive << segmentNumber_
                << percentComplete_
                << location_
                << orientation_
                << length_
                << width_
                << height_
                << depth_
                << damaged
                << deactivated
                << flaming
                << predistributed
                << smoking;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint8_t deactivated = 0;
        uint8_t flaming = 0;
        uint8_t predistributed = 0;
        uint8_t smoking = 0;
        uint32_t damaged = 0;
        archive >> segmentNumber_
                >> percentComplete_
                >> location_
                >> orientation_
                >> length_
                >> width_
                >> height_
                >> depth_
                >> damaged
                >> deactivated
                >> flaming
                >> predistributed
                >> smoking;
        deactivated_ = deactivated != 0 ;
        flaming_ = flaming != 0;
        predistributed_ = predistributed != 0;
        smoking_ = smoking != 0;
        damagedState_ = static_cast< DamageState32 >( damaged );
    }
    //@}
public:
    uint32_t segmentNumber_;
    uint32_t percentComplete_;
    WorldLocation location_;
    Orientation orientation_;
    uint16_t length_;
    uint16_t width_;
    uint16_t height_;
    uint16_t depth_;
    DamageState32 damagedState_;
    bool deactivated_;
    bool flaming_;
    bool predistributed_;
    bool smoking_;
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
    void Serialize( Archive& archive ) const
    {
        int8_t breached = static_cast< int8_t >( breachedState_ );
        archive << segmentParameters_
                << breachedLength_
                << breached;
        for( int i = 0; i < 8; ++i )
            archive << static_cast< int8_t >( segmentBreached_[i] );
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        int8_t breached;
        archive >> segmentParameters_
                >> breachedLength_
                >> breached;
        breachedState_ = static_cast< BreachedStatusEnum8 >( breached );
        for( int i = 0; i < 8; ++i )
        {
            int8_t tmp;
            archive >> tmp;
            segmentBreached_[i] =  static_cast< BreachedStatusEnum8 >( tmp );
        }
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
