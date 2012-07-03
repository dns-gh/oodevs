// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_TOGGLE_RECOGNITION_POINT_COMMAND_H
#define SWORD_PERCEPTION_TOGGLE_RECOGNITION_POINT_COMMAND_H

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
/** @class  ToggleRecognitionPointCommand
    @brief  Toggle recognition point command
*/
// Created: SLI 2012-03-22
// =============================================================================
class ToggleRecognitionPointCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ToggleRecognitionPointCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    virtual ~ToggleRecognitionPointCommand();
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
    const double size_;
    const double speed_;
    const double centerX_;
    const double centerY_;
    const int perceptionId_;
    //@}
};

}
}
#endif // SWORD_PERCEPTION_TOGGLE_RECOGNITION_POINT_COMMAND_H
