// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_AttritionData.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_AttritionData.h $
//
// *****************************************************************************

#ifndef __PHY_AttritionData_h_
#define __PHY_AttritionData_h_

namespace xml
{
    class xistream;
}

class PHY_ComposanteState;

// =============================================================================
// @class  PHY_AttritionData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_AttritionData
{
public:
              PHY_AttritionData();
     explicit PHY_AttritionData( xml::xistream& xis );
              PHY_AttritionData( const PHY_AttritionData& rhs );
              PHY_AttritionData( const PHY_AttritionData& armedAttritionData, const PHY_AttritionData& unarmedAttritionData, double armedIndividuals );
     virtual ~PHY_AttritionData();

    //! @name Operations
    //@{
    PHY_AttritionData& operator=( const PHY_AttritionData& rhs );
    double GetScore() const;
    double GetDestroyed() const;
    double GetReparableWithEvacuation() const;
    double GetReparableWithoutEvacuation() const;
    const PHY_ComposanteState& ComputeComposanteState( double urbanProtection ) const;
    //@}

private:
    double rDestroyed_;
    double rReparableWithEvacuation_;
    double rReparableWithoutEvacuation_;

    double rScore_;
};

#endif // __PHY_AttritionData_h_
