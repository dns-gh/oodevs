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

class PHY_Posture;
class PHY_ComposanteTypePion;
class PHY_RolePion_Composantes;
class PHY_HumanRank;

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
    PHY_UnitType( MIL_InputArchive& archive );
    virtual ~PHY_UnitType();

    //! @name Instanciation
    //@{
    void InstanciateComposantes( PHY_RolePion_Composantes& role ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCapacities&    GetStockCapacities               () const;
    const PHY_DotationCapacities&    GetTC1Capacities                 () const;
    const T_CommanderRepartitionMap& GetCommanderRepartition          () const;
          MT_Float                   GetPostureTime                   ( const PHY_Posture& posture ) const;
          MT_Float                   GetCoupDeSondeLength             () const;
          MT_Float                   GetCoupDeSondeWidth              () const;
          MT_Float                   GetCoefDecontaminationPerTimeStep() const;
          bool                       CanFly                           () const;
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
    };
    typedef std::map< const PHY_ComposanteTypePion*, sComposanteTypeData > T_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::const_iterator                            CIT_ComposanteTypeMap;

    typedef std::vector< uint > T_PostureTimesVector;
    //@}

private:
    //! @name Init
    //@{
    void InitializeComposantes         ( MIL_InputArchive& archive );
    void InitializeCommanderRepartition( MIL_InputArchive& archive );
    void InitializePostureTimes        ( MIL_InputArchive& archive );
    void InitializeCoupDeSonde         ( MIL_InputArchive& archive );
    void InitializeNBC                 ( MIL_InputArchive& archive );
    //@}

private:
    const PHY_DotationCapacities    dotationCapacitiesTC1_;
    const PHY_DotationCapacities    stockCapacities_;
          T_ComposanteTypeMap       composanteTypes_;
          T_PostureTimesVector      postureTimes_;
          MT_Float                  rCoupDeSondeLength_;
          MT_Float                  rCoupDeSondeWidth_;
          T_CommanderRepartitionMap commandersRepartition_;
          MT_Float                  rCoefDecontaminationPerTimeStep_;
          bool                      bCanFly_;
};

#include "PHY_UnitType.inl"

#endif // __PHY_UnitType_h_
