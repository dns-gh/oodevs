// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Subordinate_h_
#define __Subordinate_h_

#include <boost/noncopyable.hpp>
#include <vlpi/entityidentifier.h>
#include <vlutil/vlmachinetypes.h>

class DtAggregatePublisher;
class DtEntityIdentifier;
class DtEntityType;
class DtSimulationAddress;
class DtString;
class DtVrfRemoteController;

namespace plugins
{
namespace vrforces
{

// =============================================================================
/** @class  Subordinate
    @brief  Subordinate
*/
// Created: SBO 2011-03-23
// =============================================================================
class Subordinate : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Subordinate( const DtEntityType& type, DtAggregatePublisher& publisher, DtReal heading, const std::string& identifier, DtVrfRemoteController& controller, const DtSimulationAddress& address );
    virtual ~Subordinate();
    //@}

private:
    //! @name Helpers
    //@{
    static void OnCreate( const DtString& name, const DtEntityIdentifier& id, void* usr );
    //@}

private:
    //! @name Member data
    //@{
    const std::string identifier_;
    DtEntityIdentifier entityId_;
    DtVrfRemoteController& controller_;
    const DtSimulationAddress& address_;
    //@}
};

}
}

#endif // __Subordinate_h_
