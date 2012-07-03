// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_TOGGLE_LOCALIZED_PERCEPTION_COMMAND_H
#define SWORD_PERCEPTION_TOGGLE_LOCALIZED_PERCEPTION_COMMAND_H

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
/** @class  ToggleLocalizedPerceptionCommand
    @brief  Toggle localized perception command
*/
// Created: SLI 2012-03-30
// =============================================================================
class ToggleLocalizedPerceptionCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ToggleLocalizedPerceptionCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    virtual ~ToggleLocalizedPerceptionCommand();
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
    const std::string perception_;
    const int perceptionId_;
    void* localization_;
    //@}
};

}
}
#endif // SWORD_PERCEPTION_TOGGLE_LOCALIZED_PERCEPTION_COMMAND_H
