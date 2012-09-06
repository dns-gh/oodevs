// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_TOGGLE_OBJECT_DETECTION_COMMAND_H
#define SWORD_PERCEPTION_TOGGLE_OBJECT_DETECTION_COMMAND_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class TER_Localisation;

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
/** @class  ToggleObjectDetectionCommand
    @brief  Toggle object detection command
*/
// Created: SLI 2012-03-22
// =============================================================================
class ToggleObjectDetectionCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ToggleObjectDetectionCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    virtual ~ToggleObjectDetectionCommand();
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
    const double speed_;
    const double centerX_;
    const double centerY_;
    const int perceptionId_;
    boost::shared_ptr< TER_Localisation >* localization_;
    //@}
};

}
}
#endif // SWORD_PERCEPTION_TOGGLE_OBJECT_DETECTION_COMMAND_H
