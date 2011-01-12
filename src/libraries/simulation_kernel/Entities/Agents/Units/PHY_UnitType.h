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

#include "Dotations/PHY_DotationCapacities.h"

namespace xml
{
    class xistream;
}

class PHY_Posture;
class PHY_ComposanteTypePion;
class PHY_RolePion_Composantes;
class PHY_HumanRank;
class PHY_DotationLogisticType;
enum E_PionEfficiency;
enum E_CrossingHeight;

// =============================================================================
// @class  PHY_UnitType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_UnitType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< const PHY_HumanRank*, unsigned int > T_CommanderRepartitionMap;
    typedef T_CommanderRepartitionMap::const_iterator    CIT_CommanderRepartitionMap;
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
    double GetPostureTime( const PHY_Posture& posture ) const;
    double GetInstallationTime() const;
    double GetUninstallationTime() const;
    double GetCoupDeSondeLength() const;
    double GetCoupDeSondeWidth() const;
    double GetCoefDecontaminationPerTimeStep() const;
    double GetStockLogisticThresholdRatio( const PHY_DotationLogisticType& type ) const;
    bool CanFly() const;
    bool IsAutonomous() const;
    unsigned int GetPionEfficiency( E_PionEfficiency pionEfficiency ) const;
    E_CrossingHeight GetCrossingHeight() const;
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
    typedef std::map< const PHY_ComposanteTypePion*, sComposanteTypeData > T_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::const_iterator                          CIT_ComposanteTypeMap;

    typedef std::vector< unsigned int > T_PostureTimesVector;

    typedef std::vector< double >                            T_StockLogisticThresholdRatios;
    typedef T_StockLogisticThresholdRatios::const_iterator CIT_StockLogisticThresholdRatios;
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
    double rCoupDeSondeLength_;
    double rCoupDeSondeWidth_;
    T_CommanderRepartitionMap commandersRepartition_;
    double rCoefDecontaminationPerTimeStep_;
    bool bCanFly_;
    bool bIsAutonomous_; // Drones
    unsigned int nReconEfficiency_;
    unsigned int nCombatSupportEfficiency_;
    unsigned int nCombatEfficiency_;
    unsigned int nMobilitySupportEfficiency_;
    unsigned int nCounterMobilitySupportEfficiency_;
    unsigned int nProtectionSupportEfficiency_;
    unsigned int nEngineeringReconEfficiency_;
    unsigned int nUrbanAreaEfficiency_;
    E_CrossingHeight crossingHeight_;
    //@}
};

#endif // __PHY_UnitType_h_
