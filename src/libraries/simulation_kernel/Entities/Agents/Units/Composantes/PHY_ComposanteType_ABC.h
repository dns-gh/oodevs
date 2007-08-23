// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.h $
// $Author: Nld $
// $Modtime: 18/04/05 16:34 $
// $Revision: 5 $
// $Workfile: PHY_ComposanteType_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_ComposanteType_ABC_h_
#define __PHY_ComposanteType_ABC_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class PHY_Protection;
class PHY_Volume;
class MIL_Agent_ABC;

// =============================================================================
// @class  PHY_ComposanteType_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposanteType_ABC
{
    MT_COPYNOTALLOWED( PHY_ComposanteType_ABC )

public:
             PHY_ComposanteType_ABC( const std::string& strName, xml::xistream& xis );
    virtual ~PHY_ComposanteType_ABC();

    //! @name Accessors
    //@{
          ASN1T_EquipmentType GetMosID     () const;
    const PHY_Protection&      GetProtection() const;
    const PHY_Volume&          GetVolume    () const;
    const std::string&         GetName      () const;
    //@}

    //! @name Operations
    //@{
    virtual MT_Float GetDangerosity     ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& target, MT_Float rDistBtwSourceAndTarget ) const = 0;
    virtual MT_Float GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const = 0;
    virtual MT_Float GetMinRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const = 0;
    //@}

private:
    const std::string          strName_;
    const PHY_Protection*      pProtection_;
    const PHY_Volume*          pVolume_;
          ASN1T_EquipmentType nMosID_;
};

#include "PHY_ComposanteType_ABC.inl"

#endif // __PHY_ComposanteType_ABC_h_
