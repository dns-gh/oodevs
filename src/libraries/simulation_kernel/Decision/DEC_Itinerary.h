// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_KERNEL_DEC_ITINERARY_H
#define SIMULATION_KERNEL_DEC_ITINERARY_H

#include <boost/noncopyable.hpp>

namespace sword
{
    class Pathfind;
}

// DEC_Itinerary represents a directia knowledge of a simulation itinerary.
// It currently holds the item identifier but could be extended with more
// attributes for debugging purpose. Having a dedicated instance rather than
// an integer value allows actions to be done when released from directia.
//
// Note the instance is not bound to objects in PathfindComputer. We might
// want to do that in the future. For now, the other API calls with take
// the identifier and cross-check the existence of a related path themselves.
class DEC_Itinerary : private boost::noncopyable
{
public:
             DEC_Itinerary( const sword::Pathfind& path );
    virtual ~DEC_Itinerary();

    uint32_t GetId() const;

private:
    uint32_t id_;
};

#endif // SIMULATION_KERNEL_DEC_ITINERARY_H
