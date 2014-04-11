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
class MIL_Agent_ABC;
class MIL_Object_ABC;
class MIL_PopulationAttitude;
class MIL_PopulationType;
class PHY_Composante_ABC;
class PHY_ComposanteState;
class PHY_ComposanteTypePion;
class PHY_DotationCategory;
class PHY_FireDamages_Agent;
class PHY_FireResults_ABC;
class PHY_MaintenanceComposanteState;
class PHY_SensorTypeAgent_ABC;
class PHY_Volume;

// =============================================================================
// @class  PHY_RoleInterface_Composantes
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Composantes : public tools::Role_ABC
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

    typedef PHY_RoleInterface_Composantes RoleInterface;

    typedef std::vector< const PHY_Volume* > T_ComposanteVolumes;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Composantes();
    virtual ~PHY_RoleInterface_Composantes();
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean () = 0;
    //@}

    //! @name Composantes management
    //@{
    virtual void ChangeComposantesAvailability( const PHY_ComposanteTypePion& composanteType, unsigned int nNbrAvailable ) = 0;
    virtual void RepairAllComposantes( bool withLog ) = 0;
    virtual void DestroyRandomComposante() = 0;
    virtual void DestroyAllComposantes() = 0;
    //@}

    // Actions on the composante owner
    virtual void LendComposante( MIL_Agent_ABC& borrower, PHY_ComposantePion& composante ) = 0;
    virtual void RetrieveLentComposante( MIL_Agent_ABC& borrower, PHY_ComposantePion& composante ) = 0;
    virtual void RetrieveAllLentComposantes() = 0;
    virtual void ReturnAllBorrowedComposantes() = 0;

    // Notification for the beneficary
    virtual void NotifyLentComposanteReceived( MIL_Agent_ABC& lender, PHY_ComposantePion& composante ) = 0;
    virtual void NotifyLentComposanteReturned( MIL_Agent_ABC& lender, PHY_ComposantePion& composante ) = 0;
    //@}

    //$$$ Toute la partie logistique GetXXXUse() devrait être externalisée

    virtual std::size_t GetConvoyTransportersTotal() const = 0;

    //! @name Logistic - maintenance
    //@{
    virtual void PreprocessRandomBreakdowns( unsigned int /*nEndDayTimeStep*/ ) const = 0;

    virtual boost::shared_ptr< PHY_MaintenanceComposanteState > NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante ) = 0;
    virtual void NotifyComposanteBackFromMaintenance( PHY_MaintenanceComposanteState& composanteState ) = 0;
    virtual PHY_MaintenanceComposanteState* FindRequest( uint32_t request ) const = 0;

    //! @name Operations
    //@{
    virtual const PHY_Volume*         GetSignificantVolume     ( const PHY_SensorTypeAgent_ABC& sensorType ) const = 0;
    virtual       T_ComposanteVolumes GetVisibleVolumes      () const = 0;
    virtual       void                BuildKnowledgeComposantes( T_KnowledgeComposanteVector& knowledge ) const = 0;
    virtual const PHY_Composante_ABC* GetMajorComposante       () const = 0;
    virtual       double              GetOperationalState      () const = 0;
    virtual       double              GetMajorOperationalState () const = 0;
    virtual       double              GetMaxWeight             ( bool loadedWeight = false ) const = 0;
    virtual bool CanTransportDestroyed() const = 0;
    //@}

    //! @name Fire
    //@{
    virtual bool IsNeutralized              () const = 0;
    virtual void GetComposantesAbleToBeFired( PHY_Composante_ABC::T_ComposanteVector& targets, unsigned int nNbrFirers, bool bFireOnlyOnMajorComposantes = false ) const = 0;//TODO MGD Remove and use ComposantesAbleToBeFired
    virtual void GetComposantesAbleToBeFired( PHY_Composante_ABC::T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes = false ) const = 0;
    virtual void Neutralize                 () = 0;
    virtual void ApplyPopulationFire        ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result, double armedIndividuals, bool& bCanBePlundered ) = 0;
    virtual void ApplyDirectFire            ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result ) = 0;
    virtual void ApplyIndirectFire          ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result, double ratio ) = 0;
    virtual void ApplyDirectFireOnMajorComposantes( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& fireResult ) = 0;
    virtual void ApplyExplosion             ( const AttritionCapacity& capacity, PHY_FireResults_ABC& result ) = 0;
    virtual void ApplyUrbanObjectCrumbling  ( const MIL_Object_ABC& object ) = 0;
    virtual double GetDangerosity           ( const DEC_Knowledge_AgentComposante& compTarget, float rDistBtwSourceAndTarget, bool bUseAmmo ) const = 0;
    virtual double GetOnlyLoadableMaxRangeToFireOn  ( const DEC_Knowledge_Agent& target, double rWantedPH ) const = 0;
    virtual double GetMaxRangeToFireOn              ( const DEC_Knowledge_Agent& target, double rWantedPH, const PHY_DotationCategory* dotation = 0 ) const = 0;
    virtual double GetMinRangeToFireOn              ( const DEC_Knowledge_Agent& target, double rWantedPH ) const = 0;
    virtual double GetMaxRangeToFireOnActualPosture ( const DEC_Knowledge_Agent& target, double rWantedPH ) const = 0;
    virtual double GetMinRangeToFireOnActualPosture ( const DEC_Knowledge_Agent& target, double rWantedPH ) const = 0;
    virtual double GetMaxRangeToIndirectFire        ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability, bool theoric ) const = 0;
    virtual double GetMaxRangeToIndirectFire        ( ) const = 0;
    virtual double GetMinRangeToIndirectFire        ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability, bool theoric ) const = 0;
    virtual double GetMaxRangeToFire                ( const MIL_Agent_ABC&  target, double rWantedPH ) const = 0;

    virtual void ApplyContamination() = 0;
    //@}

    //! @name Notifications (internal)
    //@{
    typedef std::map< const PHY_DotationCategory*, double > T_Dotations;

    virtual void NotifyComposanteAdded( PHY_ComposantePion& composante, T_Dotations* dotations ) = 0;
    virtual T_Dotations NotifyComposanteRemoved( PHY_ComposantePion& composante ) = 0;
    virtual void NotifyComposanteChanged( PHY_ComposantePion& composante, const PHY_ComposanteState& oldState ) = 0;
    virtual void NotifyComposanteRepaired() = 0;
    //@}

    //! @name Accessors
    //@{
    virtual       bool           HasChanged() const = 0;//@TODO MGD CLEAN ALL hasChanged on interface and move private
    virtual       bool           IsUsable  () const = 0;
    virtual const MIL_Agent_ABC& GetPion   () const = 0;
    virtual       bool           IsImmobilized() const = 0;
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
    static double rOpStateDecisionalThreshold_;

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

#endif // __PHY_RoleInterface_Composantes_h_
