// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h $
// $Author: Jvt $
// $Modtime: 31/03/05 18:21 $
// $Revision: 5 $
// $Workfile: PHY_RoleInterface_Composantes.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Composantes_h_
#define __PHY_RoleInterface_Composantes_h_

#include "MT_Tools/Role_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Knowledge/DEC_Knowledge_Def.h"

class AttritionCapacity;
class HLA_UpdateFunctor;
class MIL_Agent_ABC;
class MIL_Injury_ABC;
class MIL_PopulationAttitude;
class MIL_PopulationType;
class PHY_Composante_ABC;
class PHY_ComposanteState;
class PHY_ComposanteTypePion;
class PHY_DotationCategory;
class PHY_FireDamages_Agent;
class PHY_FireResults_ABC;
class Human_ABC;
class PHY_MaintenanceComposanteState;
class PHY_SensorTypeAgent;
class PHY_Volume;

namespace firing
{
}

// =============================================================================
// @class  PHY_RoleInterface_Composantes
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Composantes : public tools::Role_ABC
                                    , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    struct T_ComposanteUse
    {
        unsigned int nNbrAvailable_;
        unsigned int nNbrUsed_;
        unsigned int nNbrTotal_; // nNbrTotal_ >= nNbrAvailable_ >= nNbrUsed_
        unsigned int nNbrLent_;
    };

    typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteUse > T_ComposanteUseMap;
    typedef T_ComposanteUseMap::const_iterator                         CIT_ComposanteUseMap;

    typedef PHY_RoleInterface_Composantes RoleInterface;

    typedef std::set< PHY_MaintenanceComposanteState* >     T_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::iterator       IT_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::const_iterator CIT_MaintenanceComposanteStateSet;

    typedef std::map< const PHY_RoleInterface_Composantes*, PHY_ComposantePion::T_ComposantePionVector > T_LoanMap;
    typedef T_LoanMap::iterator                                                 IT_LoanMap;
    typedef T_LoanMap::const_iterator                                           CIT_LoanMap;

    /*typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteTypeProperties > T_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::iterator                                         IT_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::const_iterator                                   CIT_ComposanteTypeMap;

    typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteUse > T_ComposanteUseMap;
    typedef T_ComposanteUseMap::const_iterator                         CIT_ComposanteUseMap;
   */ //@}

public:
             PHY_RoleInterface_Composantes();
    virtual ~PHY_RoleInterface_Composantes();


    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean () = 0;
    //@}

    //! @name Humans management
    //@{
//    virtual void WoundHumans  ( const PHY_HumanRank& rank, unsigned int nNbr ) = 0;
//    virtual void HealHumans   ( const PHY_HumanRank& rank, unsigned int nNbr ) = 0;
//    virtual void HealAllHumans() = 0 ;
    //@}

    //! @name Composantes management
    //@{
    virtual void ChangeComposantesAvailability( const PHY_ComposanteTypePion& composanteType, unsigned int nNbrAvailable ) = 0;
    virtual void RepairAllComposantes         () = 0;
    virtual void DestroyRandomComposante      () = 0;
    virtual void DestroyAllComposantes        () = 0;
    //@}

    // Actions on the composante owner
    virtual void LendComposante        ( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante ) = 0;
    virtual void RetrieveLentComposante( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante ) = 0;

    // Notification for the beneficary
    virtual void NotifyLentComposanteReceived( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante ) = 0;
    virtual void NotifyLentComposanteReturned( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante ) = 0;
    //@}

    //$$$ Toute la partie logistique GetXXXUse() devrait �tre externalis�e

    //! @name Logistic - maintenance
    //@{
    virtual void                            PreprocessRandomBreakdowns           ( unsigned int /*nEndDayTimeStep*/ ) const = 0;

    virtual PHY_MaintenanceComposanteState* NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante ) = 0;
    virtual void                            NotifyComposanteBackFromMaintenance  ( PHY_MaintenanceComposanteState& composanteState ) = 0;

    //! @name Operations
    //@{
    virtual const PHY_Volume*          GetSignificantVolume       ( const PHY_SensorTypeAgent& sensorType ) const = 0;
    virtual       void                 GetVisibleVolumes          ( T_ComposanteVolumeSet& volumes         ) const = 0;
    virtual       void                 BuildKnowledgeComposantes  ( T_KnowledgeComposanteVector& knowledge ) const = 0;
    virtual const PHY_Composante_ABC*  GetMajorComposante         () const = 0;
    virtual       double             GetOperationalState        () const = 0;
    virtual       double             GetMajorOperationalState   () const = 0;
    virtual       MT_Float             GetMajorComponentWeight  () const = 0;
    //@}

    //! @name Fire
    //@{
    virtual bool IsNeutralized              () const = 0;
    virtual void GetComposantesAbleToBeFired( PHY_Composante_ABC::T_ComposanteVector& targets, unsigned int nNbrFirers, bool bFireOnlyOnMajorComposantes = false ) const = 0;//TODO MGD Remove and use ComposantesAbleToBeFired
    virtual void GetComposantesAbleToBeFired( PHY_Composante_ABC::T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes = false ) const = 0;
    virtual void Neutralize                 () = 0;
    virtual void ApplyPopulationFire        ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result ) = 0;
    virtual void ApplyDirectFire            ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result ) = 0;
    virtual void ApplyIndirectFire          ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result, MT_Float ratio ) = 0;
    virtual void ApplyDirectFireOnMajorComposantes( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& fireResult ) = 0;
    virtual void ApplyExplosion             ( const AttritionCapacity& capacity, PHY_FireResults_ABC& result ) = 0;
    virtual double GetDangerosity           ( const DEC_Knowledge_AgentComposante& compTarget, float rDistBtwSourceAndTarget ) const = 0;
    virtual double GetOnlyLoadableMaxRangeToFireOn  ( const DEC_Knowledge_Agent& target, double rWantedPH ) const = 0;
    virtual double GetMaxRangeToFireOn              ( const DEC_Knowledge_Agent& target, double rWantedPH ) const = 0;
    virtual double GetMinRangeToFireOn              ( const DEC_Knowledge_Agent& target, double rWantedPH ) const = 0;
    virtual double GetMaxRangeToFireOnActualPosture ( const DEC_Knowledge_Agent& target, double rWantedPH ) const = 0;
    virtual double GetMinRangeToFireOnActualPosture ( const DEC_Knowledge_Agent& target, double rWantedPH ) const = 0;
    virtual double GetMaxRangeToIndirectFire        ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const = 0;
    virtual double GetMinRangeToIndirectFire        ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const = 0;
    virtual double GetMaxRangeToFire                ( const MIL_Agent_ABC&  target, double rWantedPH ) const = 0;

    virtual void ApplyInjury                ( MIL_Injury_ABC& injury ) = 0;
    //@}




    //! @name Notifications (internal)
    //@{
    virtual void NotifyComposanteAdded   ( PHY_ComposantePion& composante ) = 0;
    virtual void NotifyComposanteRemoved ( PHY_ComposantePion& composante ) = 0;
    virtual void NotifyComposanteChanged ( PHY_ComposantePion& composante, const PHY_ComposanteState& oldState ) = 0;
    virtual void NotifyComposanteRepaired() = 0;
    //@}

    //! @name Accessors
    //@{
    virtual       bool           HasChanged              () const = 0;//@TODO MGD CLEAN ALL hasChanged on interface and move private
    virtual       bool           IsUsable                () const = 0;
    virtual const MIL_Agent_ABC& GetPion                 () const = 0;
    //@}

    //! @name Network
    //@{
//    virtual void SendChangedState( client::UnitAttributes& asn ) const = 0;
//    virtual void SendFullState   ( client::UnitAttributes& asn ) const = 0;

    virtual void SendLogisticChangedState() const = 0;
    virtual void SendLogisticFullState   () const = 0;
    //@}

    //! @name HLA
    //@{
    virtual void Serialize( HLA_UpdateFunctor& functor ) const = 0;
    //@}

    //! @name Composante reserved
    //@{
    virtual void WoundLoadedHumans( const PHY_ComposantePion& composanteChanged, const PHY_ComposanteState& newState, PHY_FireDamages_Agent& fireDamages ) = 0;
    //@}

public:
    static double rOpStateWeightNonMajorComposante_;
    static double rOpStateWeightMajorComposante_;
    static double rMaxDangerosityDegradationByNeutralizedState_;
    static double rMaxDangerosityDegradationByOpState_;

private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const uint )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

#endif // __PHY_RoleInterface_Composantes_h_
