// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_COMPUTER_ABC_H
#define SWORD_PERCEPTION_PERCEPTION_COMPUTER_ABC_H

#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
    class PerceptionLevel;
    class SurfacesAgent_ABC;

// =============================================================================
/** @class  PerceptionComputer_ABC
    @brief  Perception computer definition
*/
// Created: SLG 2010-04-29
// =============================================================================
class PerceptionComputer_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionComputer_ABC() {}
    virtual ~PerceptionComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const PerceptionLevel& ComputePerception( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const = 0;
    //@}
};

}
}

#endif // __PerceptionComputer_ABC_h_
