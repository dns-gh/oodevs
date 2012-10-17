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

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
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
    ToggleLocalizedPerceptionCommand( const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_TOGGLE_LOCALIZED_PERCEPTION_COMMAND_H
