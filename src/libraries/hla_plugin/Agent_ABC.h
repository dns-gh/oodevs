// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Agent_ABC_h
#define plugins_hla_Agent_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    class EventListener_ABC;

// =============================================================================
/** @class  Agent_ABC
    @brief  Agent definition
*/
// Created: SLI 2011-02-04
// =============================================================================
class Agent_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent_ABC() {}
    virtual ~Agent_ABC() {}
    //@}

    //! @name Registration
    //@{
    virtual void Register( EventListener_ABC& listener ) = 0;
    virtual void Unregister( EventListener_ABC& listener ) = 0;
    //@}
};

}
}

#endif // plugins_hla_Agent_ABC_h
