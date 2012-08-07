// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PROPAGATION_FLOOD_COMMAND_H
#define SWORD_PROPAGATION_FLOOD_COMMAND_H

#include <boost/noncopyable.hpp>
#include <geometry/Types.h>
#include <memory>

namespace sword
{
namespace wrapper
{
    class View;
    class Event;
}
namespace propagation
{
    class FloodModel;
// =============================================================================
/** @class  FloodCommand
    @brief  Flood command
*/
// Created: LGY 2012-06-12
// =============================================================================
class FloodCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    FloodCommand( const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    ~FloodCommand();
    //@}

public:
    //! @name Operations
    //@{
    void Execute( const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& model ) const;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned int identifier_;
    const unsigned int depth_;
    const unsigned int radius_;
    const geometry::Point2f center_;
    std::auto_ptr< FloodModel > model_;
    //@}
};

}
}

#endif // SWORD_PROPAGATION_FLOOD_COMMAND_H
