// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Composantes/PHY_ComposanteTypeObjectData.h $
// $Author: Nld $
// $Modtime: 2/11/04 16:23 $
// $Revision: 3 $
// $Workfile: PHY_ComposanteTypeObjectData.h $
//
// *****************************************************************************

#ifndef __PHY_ComposanteTypeObjectData_h_
#define __PHY_ComposanteTypeObjectData_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class MIL_ObjectType_ABC;
class PHY_ConsumptionType;

// =============================================================================
// @class  PHY_ComposanteTypeObjectData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposanteTypeObjectData
{
public:
     PHY_ComposanteTypeObjectData( const MIL_ObjectType_ABC& objectType, xml::xistream& xis );
    ~PHY_ComposanteTypeObjectData();

    //! @name Accessors
    //@{
    bool CanConstruct() const;
    bool CanDestroy  () const;
    bool CanMine     () const;
    bool CanDemine   () const;
    bool CanBypass   ( bool bObjectIsMined ) const;

    MT_Float GetConstructionTime( MT_Float rSizeCoef ) const;
    MT_Float GetDestructionTime ( MT_Float rSizeCoef ) const;
    MT_Float GetMiningTime      () const;
    MT_Float GetDeminingTime    () const;
    MT_Float GetBypassTime      ( MT_Float rSizeCoef, bool bObjectIsMined ) const;
    MT_Float GetMaxSpeed        ( const MIL_Object_ABC& object ) const;

    const PHY_ConsumptionType* GetConsumptionMode() const;
    //@}

private:
    MT_Float rTimeBaseConstruction_;
    MT_Float rTimeBaseDestruction_;
    MT_Float rTimeConstructionCoef_;
    MT_Float rTimeDestructionCoef_;

    MT_Float rTimeMining_;
    MT_Float rTimeDemining_;

    MT_Float rCoefTimeBypass_;
    MT_Float rSpeedWithinWhenNotBypassed_;
    MT_Float rSpeedWithinWhenBypassed_;

    const PHY_ConsumptionType* pConsumptionMode_;
};

#include "PHY_ComposanteTypeObjectData.inl"

#endif // __PHY_ComposanteTypeObjectData_h_
