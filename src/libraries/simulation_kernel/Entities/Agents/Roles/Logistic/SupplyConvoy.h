// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConvoy_h_
#define __SupplyConvoy_h_

#include "SupplyConvoy_ABC.h"
#include "MT_Tools/MT_InterpolatedFunction.h"

class PHY_ComposantePion;

namespace logistic {
    class SupplyConveyor_ABC;
    class SupplyConvoyEventsObserver_ABC;
    class SupplySupplier_ABC;
    class SupplyRequestParameters_ABC;

// =============================================================================
/** @class  SupplyConvoy
    @brief  SupplyConvoy
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConvoy : public SupplyConvoy_ABC
{
public:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, double > T_Resources;
    //@}
        
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConvoy( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters );
    virtual ~SupplyConvoy();
    //@}

    //! @name Operations
    //@{
    virtual unsigned ReserveTransporters      ( const T_Resources& resources );
    virtual unsigned Setup                    ();
    virtual unsigned Load                     ();
    virtual unsigned Unload                   ();
    virtual void     SetCurrentSupplyRecipient( SupplyRecipient_ABC* supplyRecipient );
    virtual void     Supply                   ( SupplyRecipient_ABC& supplyRecipient, const PHY_DotationCategory& dotationCategory, double quantity );
    virtual void     Finish                   ();
    //@}

    //! @name Accessors
    //@{
    virtual SupplySupplier_ABC& GetTransportersProvider() const;
    //@}

protected:
    //! @name Types
    //@{
    typedef std::map< PHY_ComposantePion*, boost::shared_ptr< SupplyConveyor_ABC > > T_Conveyors;

    enum E_Action
    {
        eNone,
        eSetup,
        eLoad,
        eUnload,
        eMoveToSupplier,
        eMoveToSupplyRecipient,
        eMoveToTransportersProvider
    };
    //@}

private:
    //! @name Tools
    //@{
    void     ReserveTransporters          ( const PHY_DotationCategory& dotationCategory, double quantity );
    unsigned ComputeTimeRemainingForAction( E_Action action, MT_InterpolatedFunction< double >& timeComputer );
    template< typename T > SupplyConveyor_ABC* CreateConveyor( const T& constraint );
    //@}

protected:
    SupplyConvoyEventsObserver_ABC& eventsObserver_;
    SupplyRequestParameters_ABC& parameters_;
    SupplySupplier_ABC& supplier_;
    SupplySupplier_ABC& transportersProvider_;
    bool autoAllocateNewTransporters_;
    T_Conveyors conveyors_;
    E_Action currentAction_;
    unsigned timeRemainingForCurrentAction_;
    SupplyRecipient_ABC* currentSupplyRecipient_;
};

} // end namespace logistic

#endif // __SupplyConvoy_h_
