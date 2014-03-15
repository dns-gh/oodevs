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

namespace logistic
{
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
             SupplyConvoyReal();
    virtual ~SupplyConvoyReal();
    //@}

    //! @name Operations
    //@{
    virtual unsigned Setup                     ();
    virtual unsigned MoveToSupplier            ();
    virtual unsigned MoveToSupplyRecipient     ();
    virtual unsigned MoveToTransportersProvider();
    virtual void     Finish                    ( bool finished = true );
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
    virtual bool                 IsPushedFlow             () const;
    virtual const MIL_Agent_ABC* GetProvider              () const;
    virtual const MIL_Agent_ABC* GetReporter() const;
    //@}

    //! @name Algorithms
    //@{
    virtual void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::Id& msg ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive& archive, const unsigned int );
    //@}

private:
    //! @name Operations
    //@{
    unsigned int ReturnTimeRemainingForAction( E_Action action );
    virtual bool HasConvoy( const MIL_Agent_ABC& pion ) const;
    virtual bool IsConvoyDestroyed() const;
    //@}

private:
    MIL_AgentPion* convoyPion_;
    bool currentActionDone_;
};

} // end namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::SupplyConvoyReal )

#endif // __SupplyConvoyReal_h_
