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
#include "SupplyRecipient_ABC.h"
#include "Checkpoints/SerializationTools.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "MT_Tools/MT_InterpolatedFunction.h"
#include <tools/Map.h>

class PHY_ComposantePion;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class MT_InterpolatedFunction;

namespace logistic
{
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
    //! @name Constructors/Destructor
    //@{
             SupplyConvoy( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters );
             SupplyConvoy();
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
    virtual void     Finish                   ( bool finished = true );
    virtual void     ResetConveyors( SupplyConvoyEventsObserver_ABC& observer );
    virtual bool     HasConvoy( const MIL_Agent_ABC& pion ) const;
    virtual bool     IsFinished() const;
    virtual bool     IsConvoyDestroyed() const;
    virtual bool     IsImpossible() const;
    //@}

    //! @name Accessors
    //@{
    virtual SupplySupplier_ABC& GetTransportersProvider() const;
    virtual bool CanTransport( const PHY_DotationCategory& dotationCategory ) const;
    virtual void SetProvider( const MIL_Agent_ABC* provider );
    //@}

    //! @name Serialization
    //@{
    void serialize( MIL_CheckPointInArchive& archive, const unsigned int );
    void serialize( MIL_CheckPointOutArchive& archive, const unsigned int );
    //@}

protected:
    //! @name Types
    //@{
    typedef tools::Map< PHY_ComposantePion*, boost::shared_ptr< SupplyConveyor_ABC > > T_Conveyors;

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
    unsigned ComputeTimeRemainingForAction( E_Action action, MT_InterpolatedFunction& timeComputer );
    template< typename T > SupplyConveyor_ABC* CreateConveyor( const T& constraint );
    //@}

protected:
    SupplyConvoyEventsObserver_ABC* eventsObserver_;
    const SupplyRequestParameters_ABC* parameters_;
    SupplySupplier_ABC* supplier_;
    SupplySupplier_ABC* transportersProvider_;
    T_Conveyors conveyors_;
    E_Action currentAction_;
    unsigned timeRemainingForCurrentAction_;
    SupplyRecipient_ABC* currentSupplyRecipient_;
    MIL_Agent_ABC* provider_;
    bool autoAllocateNewTransporters_;
    bool finished_;
	bool impossible_;
};
} // end namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::SupplyConvoy )

#endif // __SupplyConvoy_h_
