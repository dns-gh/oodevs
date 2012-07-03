// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_COMPUTER_H
#define SWORD_PERCEPTION_PERCEPTION_COMPUTER_H

#include "PerceptionLevel.h"

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
// =============================================================================
/** @class  PerceptionComputer
    @brief  Perception computer
*/
// Created: SLI 2012-05-30
// =============================================================================
template< typename Visitor, typename Surface, typename Target >
class PerceptionComputer : public Visitor
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionComputer( const wrapper::View& perceiver, const Target& target )
                 : perceiver_( perceiver )
                 , target_   ( target )
                 , bestLevel_( &PerceptionLevel::notSeen_ )
             {}
    virtual ~PerceptionComputer() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Notify( const Surface& surface )
    {
        const PerceptionLevel& currentLevel = surface.ComputePerception( perceiver_, target_ );
        if( currentLevel > *bestLevel_ )
        {
            bestLevel_ = &currentLevel;
            if( bestLevel_->IsBestLevel() )
                return true;
        }
        return false;
    }
    const PerceptionLevel& GetLevel() const
    {
        return *bestLevel_;
    }
    //@}
private:
    //! @name Member data
    //@{
    const wrapper::View& perceiver_;
    const Target& target_;
    const PerceptionLevel* bestLevel_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_COMPUTER_H
