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

#include "protocol/Protocol.h"

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;
class PHY_DotationCategory;
class PHY_Protection;
class PHY_Volume;

// =============================================================================
// @class  PHY_ComposanteType_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposanteType_ABC : private boost::noncopyable
{
public:
             PHY_ComposanteType_ABC( const std::string& strName, xml::xistream& xis );
    virtual ~PHY_ComposanteType_ABC();

    //! @name Accessors
    //@{
    sword::EquipmentType  GetMosID     () const;
    const PHY_Protection& GetProtection() const;
    const PHY_Volume&     GetVolume    () const;
    const std::string&    GetName      () const;
    //@}

    //! @name Operations
    //@{
    virtual double GetDangerosity     ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& target, double rDistBtwSourceAndTarget, bool bUseAmmo ) const = 0;
    virtual double GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH, const PHY_DotationCategory* dotation ) const = 0;
    virtual double GetMinRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const = 0;
    //@}

private:
    const std::string          strName_;
    const PHY_Protection*      pProtection_;
    const PHY_Volume*          pVolume_;
    sword::EquipmentType nMosID_;
};

#endif // __PHY_ComposanteType_ABC_h_
