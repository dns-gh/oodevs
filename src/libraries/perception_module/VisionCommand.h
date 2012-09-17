// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_VISION_COMMAND_H
#define SWORD_PERCEPTION_VISION_COMMAND_H

#include <boost/noncopyable.hpp>
#include <string>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace perception
{
    class ModuleFacade;

// =============================================================================
/** @class  VisionCommand
    @brief  Vision command
*/
// Created: SLI 2012-03-29
// =============================================================================
class VisionCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             VisionCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    virtual ~VisionCommand();
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}

private:
    //! @name Member data
    //@{
    const size_t identifier_;
    const std::string mode_;
    const double x_;
    const double y_;
    //@}

};

}
}

#endif // SWORD_PERCEPTION_VISION_COMMAND_H
