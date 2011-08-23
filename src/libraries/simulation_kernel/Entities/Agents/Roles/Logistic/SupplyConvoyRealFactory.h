// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConvoyRealFactory_h_
#define __SupplyConvoyRealFactory_h_

#include "SupplyConvoyFactory_ABC.h"

namespace logistic {

// =============================================================================
/** @class  SupplyConvoyFactory_ABC
    @brief  SupplyConvoyFactory_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConvoyRealFactory : public SupplyConvoyFactory_ABC
{
public:
    //! @name Singelton
    //@{
    static const SupplyConvoyRealFactory& Instance() { return instance_; };

    //! @name Operations
    //@{
    virtual boost::shared_ptr< SupplyConvoy_ABC > Create( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters ) const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             SupplyConvoyRealFactory() {};
    virtual ~SupplyConvoyRealFactory() {};
    //@}

private:
    static SupplyConvoyRealFactory instance_;
};

} // end namespace logistic

#endif // __SupplyConvoyRealFactory_h_
