// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_Surrender.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Surrender_h_
#define __PHY_RolePion_Surrender_h_

#include "MIL.h"
#include "PHY_RoleInterface_Surrender.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"

class MIL_AgentPion;
class MIL_Army_ABC;

namespace moving
{
    class MoveComputer_ABC;
}

namespace surrender
{

// =============================================================================
// @class  PHY_RolePion_Surrender
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Surrender : public PHY_RoleInterface_Surrender
                             , public tools::AlgorithmModifier_ABC<moving::MoveComputer_ABC>
                             , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    explicit PHY_RolePion_Surrender( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Surrender();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Update    ( bool bIsDead );
    virtual void Clean     ();
    virtual void UpdateLodging( unsigned int nbrHumansCampManaged );
    virtual void Execute(moving::MoveComputer_ABC& algorithm) const;
    virtual unsigned int GetNbrHumansCampManaged() const;
    virtual unsigned int GetNbrHumansCampUnmanaged() const;
    //@}

    //! @name Main
    //@{
    virtual void NotifySurrendered      ();
    virtual void NotifySurrenderCanceled();
    virtual bool Capture                ( const MIL_AgentPion& pionTakingPrisoner );
    virtual bool Release                ();
    virtual bool Imprison               ( const MIL_Object_ABC& camp );
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_Army_ABC* GetArmySurrenderedTo() const;
    virtual       bool          IsSurrendered       () const;
    virtual       bool          IsPrisoner          () const;
    virtual       bool          IsImprisoned        ( const MIL_Object_ABC& camp );
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

private:
    //! @name Tools
    //@{
    virtual bool HasChanged() const;
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePion_Surrender )
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion&        owner_;
    bool                  bPrisoner_;
    const MIL_Object_ABC* pPrison_;
    bool                  bHasChanged_;
    unsigned int          nbrHumansLodgingManaged_;
    //@}
};

} // namespace surrender

BOOST_CLASS_EXPORT_KEY( surrender::PHY_RolePion_Surrender )
namespace surrender
{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RolePion_Surrender, MIL_AgentPion )
}

#endif // __PHY_RolePion_Surrender_h_
