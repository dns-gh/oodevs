// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_TOGGLE_SENSOR_COMMAND_H
#define SWORD_PERCEPTION_TOGGLE_SENSOR_COMMAND_H

#include <boost/noncopyable.hpp>

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
/** @class  ToggleSensorCommand
    @brief  Toggle sensor command
*/
// Created: SLI 2012-03-20
// =============================================================================
class ToggleSensorCommand : private boost::noncopyable
{
public:
    //! @name Member data
    //@{
    static const char* const name;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ToggleSensorCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    virtual ~ToggleSensorCommand();
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
    const bool isActivated_;
    //@}
};

}
}
#endif // SWORD_PERCEPTION_TOGGLE_SENSOR_COMMAND_H
