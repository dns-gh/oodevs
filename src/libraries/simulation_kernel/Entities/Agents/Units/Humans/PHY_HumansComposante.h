// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumansComposante.h $
// $Author: Jvt $
// $Modtime: 1/04/05 11:25 $
// $Revision: 6 $
// $Workfile: PHY_HumansComposante.h $
//
// *****************************************************************************

#ifndef __PHY_HumansComposante_h_
#define __PHY_HumansComposante_h_

#include "HumansComposante_ABC.h"
#include <boost/serialization/export.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class Human_ABC;
class PHY_HumanRank;
class PHY_HumanWound;
class PHY_ComposantePion;
class PHY_ComposanteState;
class PHY_FireDamages_Agent;
class PHY_RoleInterface_Composantes;
class MIL_AutomateLOG;
class MIL_Time_ABC;
class WoundEffects_ABC;

namespace sword
{
    class MissionParameters;
}

// =============================================================================
// @class  PHY_HumansComposante
// Created: JVT 2004-08-03
// =============================================================================
class PHY_HumansComposante : public HumansComposante_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_HumansComposante();
             PHY_HumansComposante( const MIL_Time_ABC& time, PHY_ComposantePion& composante, unsigned int nNbrMdr );
    virtual ~PHY_HumansComposante();
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    bool ChangeHumanRank( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound );

    virtual void ApplyContamination();
    virtual void ApplyEffect( const WoundEffects_ABC& effect );
    void ApplyWounds( const PHY_ComposanteState& newCompState, PHY_FireDamages_Agent& fireDamages );
    void HealAllHumans( bool withLog );
    void KillAllHumans();
    unsigned int HealHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange );
    unsigned int OverloadHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& newWound, bool psyop = false, bool contaminated = false );
    unsigned int WoundHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& newWound );
    void ChangeHumanState( sword::MissionParameters& msg, std::set< boost::shared_ptr< Human_ABC > >& done );
    void UpdateHumanState( sword::MissionParameters& msg, std::set< boost::shared_ptr< Human_ABC > >& done );
    void RemoveHealthyHumans( const PHY_HumanRank& rank, unsigned int humansToRemove );
    virtual void CancelLogisticRequests();

    double GetOperationalState() const;
    bool IsEmpty() const;
    //@}

    //! @name Composante notifications
    //@{
    void NotifyComposanteHandledByMaintenance();
    void NotifyComposanteBackFromMaintenance();
    void NotifyComposanteTransfered( PHY_RoleInterface_Composantes& src, PHY_RoleInterface_Composantes& dest );
    //@}

    //! @name Humans notifications
    //@{
    void NotifyHumanAdded( Human_ABC& human );
    void NotifyHumanRemoved( Human_ABC& human );
    void NotifyHumanChanged( Human_ABC& human, const Human_ABC& copyOfOldHumanState );
    //@}

    //! @name Medical
    //@{
    bool HasWoundedHumansToEvacuate() const;
    void EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 );
    //@}

    //! @name Accessors
    //@{
    const PHY_ComposantePion& GetComposante() const;
    unsigned int GetNbrUsableHumans() const;
    unsigned int GetNbrHealthyHumans( const PHY_HumanRank& rank ) const;
    //@}

private:
    void UpdateComponent() const;

private:
    PHY_ComposantePion* pComposante_;
    unsigned int nNbrUsableHumans_;
    std::vector< boost::shared_ptr< Human_ABC > > humans_;
};

BOOST_CLASS_EXPORT_KEY( PHY_HumansComposante )

#endif // __PHY_HumansComposante_h_
