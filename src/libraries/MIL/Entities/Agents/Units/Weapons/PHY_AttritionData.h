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

#include "MIL.h"

#include "MT_Tools/MT_Random.h"

class PHY_ComposanteState;

// =============================================================================
// @class  PHY_AttritionData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_AttritionData
{
public:
     PHY_AttritionData();
     PHY_AttritionData( MIL_InputArchive& archive );
     PHY_AttritionData( const PHY_AttritionData& rhs );
    ~PHY_AttritionData();

    //! @name Operations
    //@{
    PHY_AttritionData&         operator=             ( const PHY_AttritionData& rhs );
    MT_Float                   GetScore              () const;
    const PHY_ComposanteState& ComputeComposanteState() const; 
    //@}

private:
    MT_Float rDestroyedBound_;
    MT_Float rReparableWithEvacuationBound_;
    MT_Float rReparableWithoutEvacuationBound_;

    MT_Float rScore_;

private:
    static MT_Random randomGenerator_;
};

#include "PHY_AttritionData.inl"

#endif // __PHY_AttritionData_h_
