// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PERCEPTION_MODULE_EXTERNAL_PERCEPTION_COMMAND_H
#define PERCEPTION_MODULE_EXTERNAL_PERCEPTION_COMMAND_H

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
/** @class  ExternalPerceptionCommand
    @brief  External perception command
*/
// Created: SLI 2012-03-16
// =============================================================================
class ExternalPerceptionCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    ExternalPerceptionCommand( const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // PERCEPTION_MODULE_EXTERNAL_PERCEPTION_COMMAND_H
