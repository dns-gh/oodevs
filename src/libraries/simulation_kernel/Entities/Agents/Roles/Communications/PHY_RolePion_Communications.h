// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h $
// $Author: Jvt $
// $Modtime: 30/03/05 16:59 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_Communications.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Communications_h_
#define __PHY_RolePion_Communications_h_

#include "MIL.h"
#include "NetworkUnitAttributesMessageSender_ABC.h"
#include "SurrenderNotificationHandler_ABC.h"
#include "PHY_RoleInterface_Communications.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;
class MIL_KnowledgeGroup;

namespace firing
{
    class WeaponReloadingComputer_ABC;
}

namespace moving
{
    class SpeedComputer_ABC;
}
// =============================================================================
// @class  PHY_RolePion_Communications
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Communications : public PHY_RoleInterface_Communications
                                  , public tools::AlgorithmModifier_ABC< firing::WeaponReloadingComputer_ABC >
                                  , public tools::AlgorithmModifier_ABC<moving::SpeedComputer_ABC>
                                  , public network::NetworkUnitAttributesMessageSender_ABC
                                  , public surrender::SurrenderNotificationHandler_ABC
{
public:
    //! @name Static
    //@{
    static void Initialize( xml::xistream& xis );
    //@}
             PHY_RolePion_Communications();
             PHY_RolePion_Communications( MIL_Agent_ABC& entity, const bool bIsAutonomous );
    virtual ~PHY_RolePion_Communications();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean();
    virtual void NotifyCaptured();
    virtual void NotifyReleased();
    virtual void NotifySurrendered();
    virtual void NotifySurrenderCanceled();
    //@}

    //! @name Main
    //@{
    virtual bool CanReceive() const;
    virtual bool CanEmit() const;
    virtual bool CanReport() const;

    virtual void UpdateKnowledgesFromObjectPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateKnowledgesFromObjectCollision ( const DEC_Knowledge_ObjectCollision& collision );
    //@}

    //! @name Notifications
    //@{
    virtual void Jam( const MIL_Object_ABC& jammer );
    virtual void Unjam( const MIL_Object_ABC& jammer );

    virtual void ActivateBlackout();
    virtual void ActivatePartialBlackout( bool report = true );
    virtual void DeactivateBlackout();
    //@}

    //! @name Algoritms visitors
    //@{
    virtual void Execute( firing::WeaponReloadingComputer_ABC& algorithm ) const;
    virtual void Execute( moving::SpeedComputer_ABC& algorithm) const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

    //! @name Accessors
    //@{
    boost::shared_ptr< MIL_KnowledgeGroup > GetJammedKnowledgeGroup() const;
    bool IsJammed() const;
    bool IsInEmissionBlackout() const;
    bool IsInReceptionBlackout() const;
    //@}

private:
    //! @name Tools
    //@{
    void CopyKnowledgeGroup();
    void CopyKnowledgeGroupPartial();
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC*      owner_;
    std::set< const MIL_Object_ABC* > jammers_;
    bool                bBlackoutEmittedActivated_;
    bool                bBlackoutReceivedActivated_;
    bool                bBlackoutEmittedReport_;
    bool                bHasChanged_;
    bool                bSilentBeforeCapture_;
    bool                bIsAutonomous_;
    boost::shared_ptr< MIL_KnowledgeGroup > pJammingKnowledgeGroup_;
    static double       rCoefSpeedModificator_;
    static double       rCoefReloadingTimeModificator_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Communications )

#endif // __PHY_RolePion_Communications_h_
