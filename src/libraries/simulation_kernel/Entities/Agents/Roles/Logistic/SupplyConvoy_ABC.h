// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConvoy_ABC_h_
#define __SupplyConvoy_ABC_h_

#include <tools/Map.h>

class PHY_DotationCategory;
class MIL_Agent_ABC;
class MIL_AgentPion;

namespace sword
{
    class UnitId;
}

namespace logistic
{
    class SupplyRecipient_ABC;
    class SupplySupplier_ABC;
    class SupplyConvoyEventsObserver_ABC;

// =============================================================================
/** @class  SupplyConvoy_ABC
    @brief  SupplyConvoy_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConvoy_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_DotationCategory*, double > T_Resources;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             SupplyConvoy_ABC() {}
    virtual ~SupplyConvoy_ABC() {}
    //@}

    //! @name Operations - return the estimated time remaining for the action
    //@{
    virtual unsigned ReserveTransporters       ( const T_Resources& resources ) = 0;
    virtual unsigned Setup                     () = 0;
    virtual unsigned MoveToSupplier            () = 0;
    virtual unsigned MoveToSupplyRecipient     () = 0;
    virtual unsigned MoveToTransportersProvider() = 0;
    virtual unsigned Load                      () = 0;
    virtual unsigned Unload                    () = 0;
    virtual void     SetCurrentSupplyRecipient ( SupplyRecipient_ABC* supplyRecipient ) = 0;
    virtual void     Supply                    ( SupplyRecipient_ABC& supplyRecipient, const PHY_DotationCategory& dotationCategory, double quantity ) = 0;
    virtual void     Finish                    ( bool finished = true ) = 0;
    virtual void     ResetConveyors( SupplyConvoyEventsObserver_ABC& observer ) = 0;
    virtual bool     HasConvoy( const MIL_Agent_ABC& pion ) const = 0;
    virtual bool     IsConvoyDestroyed() const = 0;
    virtual bool     IsFinished() const = 0;
    virtual bool     IsImpossible() const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual SupplySupplier_ABC& GetTransportersProvider() const = 0;
    virtual bool CanTransport( const PHY_DotationCategory& dotationCategory ) const = 0;
    virtual void SetProvider( const MIL_Agent_ABC* provider ) = 0;
    virtual const MIL_Agent_ABC* GetReporter() const = 0;
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::Id& msg ) const = 0;
    //@}

    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
};

} // end namespace logistic

#endif // __SupplyConvoy_ABC_h_
