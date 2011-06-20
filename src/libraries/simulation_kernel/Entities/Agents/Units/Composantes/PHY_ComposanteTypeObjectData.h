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
    PHY_ComposanteTypeObjectData( xml::xistream& xis );
    virtual ~PHY_ComposanteTypeObjectData();

    //! @name Accessors
    //@{
    bool CanConstruct() const;
    bool CanDestroy  () const;
    bool CanMine     () const;
    bool CanDemine   () const;
    bool CanBypass   ( bool bObjectIsMined ) const;
    bool CanExtinguish() const;
    bool CanRemoveFromPath( bool bObjectIsMined ) const;

    double GetConstructionTime( double rSizeCoef ) const;
    double GetDestructionTime ( double rSizeCoef ) const;
    double GetMiningTime      () const;
    double GetDeminingTime    () const;
    double GetExtinguishingTime() const;
    double GetBypassTime      ( double rSizeCoef, bool bObjectIsMined ) const;
    double GetMaxSpeed        ( const MIL_Object_ABC& object ) const;

    const PHY_ConsumptionType* GetConsumptionMode() const;
    //@}

private:
    double rTimeBaseConstruction_;
    double rTimeBaseDestruction_;
    double rTimeConstructionCoef_;
    double rTimeDestructionCoef_;

    double rTimeMining_;
    double rTimeDemining_;
    double rTimeExtinguishing_;

    double rCoefTimeBypass_;
    double rSpeedWithinWhenNotBypassed_;
    double rSpeedWithinWhenBypassed_;

    const PHY_ConsumptionType* pConsumptionMode_;
};

#endif // __PHY_ComposanteTypeObjectData_h_
