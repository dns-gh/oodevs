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
#include "PHY_RoleInterface_Communications.h"
#include "NetworkUnitAttributesMessageSender_ABC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"

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
{
public:
    //! @name Static
    //@{
    static void Initialize( xml::xistream& xis );
    //@}

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
    //@}

    //! @name Main
    //@{
    virtual bool CanReceive() const;
    virtual bool CanEmit() const;

    virtual void UpdateKnowledgesFromObjectPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateKnowledgesFromObjectCollision ( const DEC_Knowledge_ObjectCollision& collision );
    //@}

    //! @name Notifications
    //@{
    virtual void Jam( const MIL_Object_ABC& jammer );
    virtual void Unjam( const MIL_Object_ABC& jammer );

    virtual void ActivateBlackout();
    virtual void ActivatePartialBlackout();
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
    MIL_KnowledgeGroup& GetKnowledgeGroup() const;
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePion_Communications )
    //@}

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_Object_ABC* > T_JammerSet;
    typedef T_JammerSet::const_iterator     CIT_JammerSet;
    //@}

private:
    //! @name Tools
    //@{
    void CopyKnowledgeGroup();
    void CopyKnowledgeGroupPartial();
    //@}

    //! @name Member data
    //@{
    MIL_Agent_ABC&      owner_;
    T_JammerSet         jammers_;
    bool                bBlackoutEmmittedActivated_;
    bool                bBlackoutReceivedActivated_;
    bool                bHasChanged_;
    const bool          bIsAutonomous_;
    MIL_KnowledgeGroup* pJammingKnowledgeGroup_;
    //@}

    //! @name Static member data
    //@{
    static double       rCoefSpeedModificator_;
    static double       rCoefReloadingTimeModificator_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Communications )

#endif // __PHY_RolePion_Communications_h_
