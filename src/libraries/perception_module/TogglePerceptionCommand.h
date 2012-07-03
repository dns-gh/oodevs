// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_TOGGLE_PERCEPTION_COMMAND_H
#define SWORD_PERCEPTION_TOGGLE_PERCEPTION_COMMAND_H

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
/** @class  TogglePerceptionCommand
    @brief  Toggle perception command
*/
// Created: SLI 2012-03-20
// =============================================================================
class TogglePerceptionCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TogglePerceptionCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    virtual ~TogglePerceptionCommand();
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& model ) const;
    //@}

private:
    //! @name Member data
    //@{
    const size_t identifier_;
    const std::string perception_;
    const bool isActivated_;
    //@}
};

}
}
#endif // SWORD_PERCEPTION_TOGGLE_PERCEPTION_COMMAND_H
