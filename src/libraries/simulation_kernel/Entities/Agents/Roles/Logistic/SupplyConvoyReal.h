// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConvoyReal_h_
#define __SupplyConvoyReal_h_

#include "SupplyConvoy.h"
#include "SupplyConvoyReal_ABC.h"
#include "MT_Tools/MT_Vector2DTypes.h"

class MIL_AgentPion;

namespace logistic {

// =============================================================================
/** @class  SupplyConvoyReal
    @brief  SupplyConvoyReal
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConvoyReal : public SupplyConvoy
                       , public SupplyConvoyReal_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConvoyReal( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters );
    virtual ~SupplyConvoyReal();
    //@}

    //! @name Operations
    //@{
    virtual unsigned Setup                     ();
    virtual unsigned MoveToSupplier            ();
    virtual unsigned MoveToSupplyRecipient     ();
    virtual unsigned MoveToTransportersProvider();
    virtual void     Finish                    ();
    //@}

    //! @name Events
    //@{
    virtual void NotifyMovedToSupplier            ();
    virtual void NotifyMovedToTransportersProvider();
    virtual void NotifyMovedToSupplyRecipient     ();
    virtual void NotifyTransporterDestroyed       ( PHY_ComposantePion& transporter );
    virtual void NotifyConvoyEndMission           ();
    //@}

    //! @name Accessors
    //@{
    virtual int                  GetCurrentAction         () const;
    virtual SupplyRecipient_ABC* GetCurrentSupplyRecipient() const;
    virtual SupplySupplier_ABC&  GetSupplier              () const;
    virtual SupplySupplier_ABC&  GetTransportersProvider  () const;
    virtual const T_PointVector* GetPathToNextDestination () const;
    //@}

    //! @name Algorithms
    //@{
    virtual void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::UnitId& msg ) const;
    //@}

private:
    //! @name Operations
    //@{
    unsigned SupplyConvoyReal::ReturnTimeRemainingForAction( E_Action action );
    //@}

private:
    MIL_AgentPion* convoyPion_;
    bool currentActionDone_;
};

} // end namespace logistic

#endif // __SupplyConvoyReal_h_
