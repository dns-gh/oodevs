// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_ZO_PERCEPTION_COMPUTER_H
#define SWORD_PERCEPTION_ZO_PERCEPTION_COMPUTER_H

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
/** @class  ZOPerceptionComputer
    @brief  ZO Perception Computer
*/
// Created: SLG 2010-04-29
// =============================================================================
class ZOPerceptionComputer
{
public:
    //! @name Constructors/Destructor
    //@{
             ZOPerceptionComputer();
    virtual ~ZOPerceptionComputer();
    //@}

    //! @name Operations
    //@{
    const PerceptionLevel& ComputePerception( const wrapper::View& source, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_ZO_PERCEPTION_COMPUTER_H
