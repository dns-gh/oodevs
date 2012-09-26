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
#include <boost/function.hpp>

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
template< typename Visitor, typename Surface >
class PerceptionComputer : public Visitor
{
public:
    //! @name Constructors/Destructor
    //@{
    typedef boost::function< const PerceptionLevel&( const Surface& ) > T_Computer;
    explicit PerceptionComputer( T_Computer computer )
                 : computer_ ( computer )
                 , bestLevel_( &PerceptionLevel::notSeen_ )
             {}
    virtual ~PerceptionComputer() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Notify( const Surface& surface )
    {
        const PerceptionLevel& currentLevel = computer_( surface );
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
    T_Computer computer_;
    const PerceptionLevel* bestLevel_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_COMPUTER_H
