// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationConsumptions.h $
// $Author: Jvt $
// $Modtime: 7/02/05 11:24 $
// $Revision: 2 $
// $Workfile: PHY_DotationConsumptions.h $
//
// *****************************************************************************

#ifndef __PHY_DotationConsumptions_h_
#define __PHY_DotationConsumptions_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class PHY_DotationType;
class PHY_DotationCategory;
class PHY_DotationConsumption;
class PHY_DotationGroupContainer;

// =============================================================================
// @class  PHY_DotationConsumptions
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationConsumptions
{
    MT_COPYNOTALLOWED( PHY_DotationConsumptions )

public:
    //! @name types
    //@{
    typedef std::map< const PHY_DotationCategory*, MT_Float > T_ConsumptionValue;
    typedef T_ConsumptionValue::const_iterator                CIT_ConsumptionValue;  
    //@}

public:
     PHY_DotationConsumptions( const std::string& strParentTagName, xml::xistream& xis );
    ~PHY_DotationConsumptions();

    //! @name Accessors
    //@{
    void AddConsumptionValues( T_ConsumptionValue& ) const;
    //@}
    
    //! @name Operations
    //@{
    bool RegisterConsumptionReservations( PHY_DotationGroupContainer& container ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, PHY_DotationConsumption* > T_DotationConsumptionMap;
    typedef T_DotationConsumptionMap::const_iterator                          CIT_DotationConsumptionMap;  
    //@}

private:
    //! @name Helpers
    //@{
    void ReadDotation( xml::xistream& xis );
    //@}
private:
    T_DotationConsumptionMap dotationConsumptions_;
};

#include "PHY_DotationConsumptions.inl"

#endif // __PHY_DotationConsumptions_h_
