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

#include "MIL.h"

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

// =============================================================================
// @class  PHY_UnitType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_UnitType
{
    MT_COPYNOTALLOWED( PHY_UnitType )

public:
    //! @name Types
    //@{
    typedef std::map< const PHY_HumanRank*, uint >   T_CommanderRepartitionMap;
    typedef T_CommanderRepartitionMap::const_iterator CIT_CommanderRepartitionMap;
    //@}

public:
    PHY_UnitType( xml::xistream& xis );
    virtual ~PHY_UnitType();

    //! @name Instanciation
    //@{
    void InstanciateComposantes( PHY_RolePion_Composantes& role ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCapacities&    GetTC1Capacities                 () const;
    const T_CommanderRepartitionMap& GetCommanderRepartition          () const;
          MT_Float                   GetPostureTime                   ( const PHY_Posture& posture ) const;
          MT_Float                   GetInstallationTime              () const;
          MT_Float                   GetUninstallationTime            () const;
          MT_Float                   GetCoupDeSondeLength             () const;
          MT_Float                   GetCoupDeSondeWidth              () const;
          MT_Float                   GetCoefDecontaminationPerTimeStep() const;
          MT_Float                   GetStockLogisticThresholdRatio   ( const PHY_DotationLogisticType& type ) const;
          bool                       CanFly                           () const;
          bool                       IsAutonomous                     () const;
    //@}

private:
    //! @name Types
    //@{
    struct sComposanteTypeData
    {
        sComposanteTypeData();
        uint nNbr_;
        bool bMajor_;
        bool bLoadable_;
        bool bCanBePartOfConvoy_;
        uint nNbrHumanInCrew_;
    };
    typedef std::map< const PHY_ComposanteTypePion*, sComposanteTypeData > T_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::const_iterator                            CIT_ComposanteTypeMap;

    typedef std::vector< uint > T_PostureTimesVector;

    typedef std::vector< MT_Float >                        T_StockLogisticThresholdRatios;
    typedef T_StockLogisticThresholdRatios::const_iterator CIT_StockLogisticThresholdRatios;
    //@}

private:
    //! @name Init
    //@{
    void InitializeComposantes                 ( xml::xistream& xis );
    void InitializeCommanderRepartition        ( xml::xistream& xis );
    void InitializePostureTimes                ( xml::xistream& xis );
    void InitializeCoupDeSonde                 ( xml::xistream& xis );
    void InitializeNBC                         ( xml::xistream& xis );
    void InitializeInstallationTimes           ( xml::xistream& xis );
    void InitializeStockLogisticThresholdRatios( xml::xistream& xis );
    //@}
    //! @name Helpers
    //@{
    void ReadStock    ( xml::xistream& xis );
    void ReadEquipment( xml::xistream& xis );
    void ReadCrewRank ( xml::xistream& xis );
    void ReadPosture  ( xml::xistream& xis );
    void ReadSetup    ( xml::xistream& xis );
    void ReadDrill    ( xml::xistream& xis );
    //@}

private:
    const PHY_DotationCapacities         dotationCapacitiesTC1_;
          T_StockLogisticThresholdRatios stockLogisticThresholdRatios_;
          T_ComposanteTypeMap            composanteTypes_;
          T_PostureTimesVector           postureTimes_;
          MT_Float                       rInstallationTime_;
          MT_Float                       rUninstallationTime_;
          MT_Float                       rCoupDeSondeLength_;
          MT_Float                       rCoupDeSondeWidth_;
          T_CommanderRepartitionMap      commandersRepartition_;
          MT_Float                       rCoefDecontaminationPerTimeStep_;
          bool                           bCanFly_;
          bool                           bIsAutonomous_; // Drones
};

#include "PHY_UnitType.inl"

#endif // __PHY_UnitType_h_
