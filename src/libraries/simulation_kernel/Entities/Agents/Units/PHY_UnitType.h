// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/PHY_UnitType.h $
// $Author: Nld $
// $Modtime: 7/03/05 10:43 $
// $Revision: 5 $
// $Workfile: PHY_UnitType.h $
//
// *****************************************************************************

#ifndef __PHY_UnitType_h_
#define __PHY_UnitType_h_

#include "Postures/PostureTime_ABC.h"
#include "Dotations/PHY_DotationCapacities.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include "ENT/ENT_Enums.h"
#include <tools/Map.h>

namespace xml
{
    class xistream;
}

class PHY_Posture;
class PHY_ComposanteTypePion;
class PHY_RolePion_Composantes;
class PHY_HumanRank;
class PHY_NbcSuit;
class PHY_Dotation;
class PHY_DotationLogisticType;

// =============================================================================
// @class  PHY_UnitType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_UnitType : public posture::PostureTime_ABC
{
public:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_HumanRank*, unsigned int > T_CommanderRepartitionMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_UnitType( xml::xistream& xis );
    virtual ~PHY_UnitType();
    //@}

    //! @name Instanciation
    //@{
    void InstanciateComposantes( PHY_RolePion_Composantes& role ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCapacities& GetTC1Capacities() const;
    const T_CommanderRepartitionMap& GetCommanderRepartition() const;
    virtual double GetPostureSetupTime( const PHY_Posture& posture ) const;
    virtual double GetPostureTearDownTime( const PHY_Posture& posture ) const;
    double GetInstallationTime() const;
    double GetUninstallationTime() const;
    unsigned int GetFootprintRadius() const;
    double GetSpeedModifier() const;
    double GetCoupDeSondeLength() const;
    double GetCoupDeSondeWidth() const;
    double GetCoefDecontaminationPerTimeStep() const;
    double GetStockLogisticThresholdRatio( const PHY_DotationLogisticType& type ) const;
    double GetDefaultLowThreshold( const PHY_DotationCategory& category ) const;
    double GetDefaultHighThreshold( const PHY_DotationCategory& category ) const;
    double GetNormalizedConsumption( const PHY_DotationCategory& category ) const;
    bool CanFly() const;
    unsigned int GetStandardFlyingHeight() const;
    unsigned int GetTacticalFlyingHeight() const;
    bool IsAutonomous() const;
    unsigned int GetPionEfficiency( E_PionEfficiency pionEfficiency ) const;
    E_CrossingHeight GetCrossingHeight() const;
    const PHY_NbcSuit& GetNbcSuit() const;
    bool IsStockLogisticTypeDefined( const PHY_DotationLogisticType& type ) const;
    double GetResourceCapacityWithAviationQuotas( E_AviationRange aviationRange, const PHY_Dotation& dotation ) const;
    //@}

private:
    //! @name Types
    //@{
    struct sComposanteTypeData
    {
        sComposanteTypeData();
        unsigned int nNbr_;
        bool bMajor_;
        bool bLoadable_;
        bool bCanBePartOfConvoy_;
        unsigned int nNbrHumanInCrew_;
    };
    typedef tools::Map< const PHY_ComposanteTypePion*, sComposanteTypeData > T_ComposanteTypeMap;
    typedef std::vector< std::pair< unsigned int, unsigned int > > T_PostureTimesVector;
    typedef std::map< const PHY_DotationLogisticType*, double > T_StockLogisticThresholdRatios;
    //@}

private:
    //! @name Init
    //@{
    void InitializeComposantes( xml::xistream& xis );
    void InitializeCommanderRepartition( xml::xistream& xis );
    void InitializePostureTimes( xml::xistream& xis );
    void InitializeCoupDeSonde( xml::xistream& xis );
    void InitializeNBC( xml::xistream& xis );
    void InitializeInstallationTimes( xml::xistream& xis );
    void InitializeStockLogisticThresholdRatios( xml::xistream& xis );
    void InitializeEfficiencies( xml::xistream& xis );
    void InitializeCrossingHeight( xml::xistream& xis );
    //@}

    //! @name Helpers
    //@{
    void ReadStock( xml::xistream& xis );
    void ReadEquipment( xml::xistream& xis );
    void ReadCrewRank( xml::xistream& xis );
    void ReadPosture( xml::xistream& xis );
    void ReadSetup( xml::xistream& xis );
    void ReadDrill( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const PHY_DotationCapacities dotationCapacitiesTC1_;
    T_StockLogisticThresholdRatios stockLogisticThresholdRatios_;
    T_ComposanteTypeMap composanteTypes_;
    T_PostureTimesVector postureTimes_;
    double rInstallationTime_;
    double rUninstallationTime_;
    unsigned int footprintRadius_;
    double rSpeedModifier_;
    double rCoupDeSondeLength_;
    double rCoupDeSondeWidth_;
    T_CommanderRepartitionMap commandersRepartition_;
    double rCoefDecontaminationPerTimeStep_;
    bool bCanFly_;
    bool bIsAutonomous_; // Drones
    unsigned int standardFlyingHeight_;
    unsigned int tacticalFlyingHeight_;
    unsigned int nReconEfficiency_;
    unsigned int nCombatSupportEfficiency_;
    unsigned int nCombatEfficiency_;
    unsigned int nMobilitySupportEfficiency_;
    unsigned int nCounterMobilitySupportEfficiency_;
    unsigned int nProtectionSupportEfficiency_;
    unsigned int nEngineeringReconEfficiency_;
    unsigned int nUrbanAreaEfficiency_;
    E_CrossingHeight crossingHeight_;
    boost::shared_ptr< const PHY_NbcSuit > suit_;
    std::set< const PHY_DotationLogisticType* > definedStockLogisticTypes_;
    //@}
};

#endif // __PHY_UnitType_h_
