// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h $
// $Author: Jvt $
// $Modtime: 31/03/05 10:20 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_Reinforcement.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Reinforcement_h_
#define __PHY_RoleInterface_Reinforcement_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>

namespace client
{
    class UnitAttributes;
}

class MIL_AgentPion;

// =============================================================================
// @class  PHY_RoleInterface_Reinforcement
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Reinforcement : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Reinforcement RoleInterface;

    typedef std::set< MIL_AgentPion* >   T_PionSet;
    typedef T_PionSet::const_iterator  CIT_PionSet;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Reinforcement() {}
    virtual ~PHY_RoleInterface_Reinforcement() {}
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;
    virtual bool HasChanged() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual bool Reinforce( MIL_AgentPion& pion ) = 0;
    virtual void CancelReinforcement() = 0;
    virtual bool IsReinforcing() const = 0;
    virtual bool IsReinforced() const = 0;
    virtual bool IsReinforcedBy( MIL_AgentPion& pion ) const = 0;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const = 0;
    virtual void SendFullState( client::UnitAttributes& msg ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const T_PionSet& GetReinforcements() const = 0;
    virtual bool CanReinforce() const = 0;
    virtual bool CanBeReinforced() const = 0;
    //@}

private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

#endif // __PHY_RoleInterface_Reinforcement_h_
