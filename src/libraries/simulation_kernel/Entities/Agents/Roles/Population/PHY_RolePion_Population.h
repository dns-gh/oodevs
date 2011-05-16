// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RolePion_Population.h $
// $Author: Jvt $
// $Modtime: 6/04/05 16:58 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_Population.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Population_h_
#define __PHY_RolePion_Population_h_

#include "PHY_RoleInterface_Population.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"

#include "MT_Tools/AlgorithmModifier_ABC.h"

class MIL_Agent_ABC;

namespace firing
{
    class WeaponReloadingComputer_ABC;
}
namespace moving
{
    class SpeedComputer_ABC;
}

// =============================================================================
// @class  PHY_RolePion_Population
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Population : public PHY_RoleInterface_Population
                              , public tools::AlgorithmModifier_ABC<moving::SpeedComputer_ABC>
                              , public tools::AlgorithmModifier_ABC< firing::WeaponReloadingComputer_ABC >
{
public:
    explicit PHY_RolePion_Population( MIL_Agent_ABC& pion );
    virtual ~PHY_RolePion_Population();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    virtual void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Execute( firing::WeaponReloadingComputer_ABC& algorithm ) const;

    virtual bool IsInvulnerable        () const;

    virtual double GetCollidingPopulationDensity() const;
    virtual bool HasCollision() const;
    //@}


private:
    MIL_Agent_ABC& pion_;
    mutable double rPopulationDensity_;
    bool           bHasChanged_;
    mutable bool   bDensityComputed_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Population* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Population* role, const unsigned int /*version*/ );

};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Population )

#endif // __PHY_RolePion_Population_h_
