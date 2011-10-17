// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportationController_ABC_h
#define plugins_hla_TransportationController_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct ListOfTransporters; // $$$$ _RC_ SLI 2011-10-12: erk
}
    class TransportationListener_ABC;
    class TransportedUnits_ABC;

// =============================================================================
/** @class  TransportationController_ABC
    @brief  Transportation controller definition
*/
// Created: SLI 2011-10-07
// =============================================================================
class TransportationController_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportationController_ABC() {}
    virtual ~TransportationController_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( TransportationListener_ABC& listener ) = 0;
    virtual void Unregister( TransportationListener_ABC& listener ) = 0;
    virtual void OfferReceived( unsigned int context, bool fullOffer, const std::string& provider, const interactions::ListOfTransporters& listOfTransporters ) = 0;
    virtual void ServiceStarted( unsigned int context ) = 0;
    virtual void NotifyEmbarkationStatus( unsigned int context, const std::string& transporterCallsign, const TransportedUnits_ABC& transportedUnits ) = 0;
    virtual void NotifyDisembarkationStatus( unsigned int context, const std::string& transporterCallsign, const TransportedUnits_ABC& transportedUnits ) = 0;
    virtual void ServiceComplete( unsigned int context, const std::string& provider ) = 0;
    //@}
};

}
}

#endif // plugins_hla_TransportationController_ABC_h
