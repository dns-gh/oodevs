// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConvoyFactory_ABC_h_
#define __SupplyConvoyFactory_ABC_h_

namespace logistic {
    class SupplyConvoy_ABC;
    class SupplyConvoyEventsObserver_ABC;
    class SupplySupplier_ABC;
    class SupplyRequestParameters_ABC;

// =============================================================================
/** @class  SupplyConvoyFactory_ABC
    @brief  SupplyConvoyFactory_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConvoyFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConvoyFactory_ABC() {};
    virtual ~SupplyConvoyFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< SupplyConvoy_ABC > Create( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters ) const = 0;
    //@}
};

} // end namespace logistic

#endif // __SupplyConvoyFactory_ABC_h_
