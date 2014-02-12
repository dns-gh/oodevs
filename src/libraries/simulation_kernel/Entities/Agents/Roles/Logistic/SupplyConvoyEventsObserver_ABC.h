// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConvoyEventsObserver_ABC_h_
#define __SupplyConvoyEventsObserver_ABC_h_

class PHY_DotationCategory;

namespace logistic
{
// =============================================================================
/** @class  SupplyConvoyEventsObserver_ABC
    @brief  SupplyConvoyEventsObserver_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConvoyEventsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConvoyEventsObserver_ABC() {}
    virtual ~SupplyConvoyEventsObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnResourceAssignedToConvoy( const PHY_DotationCategory& dotationCategory, double quantity ) = 0;
    virtual void OnResourceRemovedFromConvoy( const PHY_DotationCategory& dotationCategory, double quantity ) = 0;
    virtual void OnConvoyEndMission() = 0;
    //@}

    template< typename Archive > void serialize( Archive&, const unsigned int )
    {}
};

}

#endif // __SupplyConveyor_ABC_h_
