// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RoleAction_MovingUnderground_h_
#define __PHY_RoleAction_MovingUnderground_h_

#include "MIL.h"
#include "NetworkUnitAttributesMessageSender_ABC.h"
#include "MT_Tools/Role_ABC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"

namespace detection
{
    class DetectionComputer_ABC;
}

class DEC_Knowledge_Object;
class MIL_Agent_ABC;

// =============================================================================
/** @class  PHY_RoleAction_MovingUnderground
    @brief  PHY_RoleAction_MovingUnderground
*/
// Created: JSR 2011-06-08
// =============================================================================
class PHY_RoleAction_MovingUnderground : public tools::Role_ABC
                                       , public tools::AlgorithmModifier_ABC< detection::DetectionComputer_ABC >
                                       , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_MovingUnderground RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RoleAction_MovingUnderground( MIL_Agent_ABC& pion );
    virtual ~PHY_RoleAction_MovingUnderground();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean();
    virtual void Execute( detection::DetectionComputer_ABC& algorithm ) const;

    bool InitializeUndergroundMoving( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    bool Run();
    bool CanExitFromCurrentLocation() const;
    bool PreparingToHide() const;
    bool IsUnderground() const;
    double EstimatedUndergroundTime( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge ) const;
    bool HideInUndergroundNetwork( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    void GetOutFromUndergroundNetwork();
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

private:
    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RoleAction_MovingUnderground )
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC&                            owner_;
    std::string                               currentNetwork_;
    boost::shared_ptr< DEC_Knowledge_Object > pCurrentLocation_;
    boost::shared_ptr< DEC_Knowledge_Object > pDestination_;
    double                                    transferTime_;
    double                                    speed_;
    bool                                      preparingToHide_;
    bool                                      bHasChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RoleAction_MovingUnderground )
INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RoleAction_MovingUnderground, MIL_Agent_ABC )

#endif // __PHY_RoleAction_MovingUnderground_h_
