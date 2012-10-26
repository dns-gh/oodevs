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
    ToggleRecognitionPointCommand( const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Destroy( const wrapper::View& parameters, const wrapper::View& model ) const;
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_TOGGLE_RECOGNITION_POINT_COMMAND_H
