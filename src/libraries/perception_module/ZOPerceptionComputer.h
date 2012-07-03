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

#include "PerceptionComputer_ABC.h"

namespace sword
{
namespace perception
{
// =============================================================================
/** @class  ZOPerceptionComputer
    @brief  ZO Perception Computer
*/
// Created: SLG 2010-04-29
// =============================================================================
class ZOPerceptionComputer : public PerceptionComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ZOPerceptionComputer();
    virtual ~ZOPerceptionComputer();
    //@}

    //! @name Operations
    //@{
    virtual const PerceptionLevel& ComputePerception( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_ZO_PERCEPTION_COMPUTER_H
