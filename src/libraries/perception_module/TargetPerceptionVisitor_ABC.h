// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_TARGET_PERCEPTION_VISITOR_ABC_H
#define SWORD_PERCEPTION_TARGET_PERCEPTION_VISITOR_ABC_H

#include <boost/noncopyable.hpp>

namespace sword
{
namespace perception
{
// =============================================================================
/** @class  TargetPerceptionVisitor_ABC
    @brief  Target perception visitor definition
*/
// Created: SLI 2012-05-30
// =============================================================================
class TargetPerceptionVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TargetPerceptionVisitor_ABC() {}
    virtual ~TargetPerceptionVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Notify( std::size_t targetIdentifier, unsigned int delay ) = 0;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_TARGET_PERCEPTION_VISITOR_ABC_H
