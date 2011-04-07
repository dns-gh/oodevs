// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Listener_ABC_h_
#define __Listener_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class MissionType;
    class OrderParameter;
}

namespace mission_tester
{
// =============================================================================
/** @class  Listener_ABC
    @brief  Listener_ABC
*/
// Created: PHC 2011-04-06
// =============================================================================
class Listener_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Listener_ABC() {};
    virtual ~Listener_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void MissionCreated( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const = 0;
    virtual void ConnectionSucceeded( const std::string& endpoint ) const = 0;
    virtual void AuthenticationSucceeded( const std::string& profile ) const = 0;
    virtual void ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::MissionType& mission, const kernel::OrderParameter& parameter ) const = 0;
    //@}
};
}

#endif // __Listener_ABC_h_
