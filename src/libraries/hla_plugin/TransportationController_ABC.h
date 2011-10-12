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
    class TransportationListener_ABC;

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
    virtual void OfferReceived( unsigned int context, bool fullOffer, const std::string& provider ) = 0;
    //@}
};

}
}

#endif // plugins_hla_TransportationController_ABC_h
