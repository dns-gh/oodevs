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

#include <tools/Map.h>

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;
class PHY_DotationConsumption;
class PHY_DotationGroupContainer;

// =============================================================================
// @class  PHY_DotationConsumptions
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationConsumptions : private boost::noncopyable
{
public:
    //! @name types
    //@{
    typedef tools::Map< const PHY_DotationCategory*, double > T_ConsumptionValue;
    //@}

public:
             PHY_DotationConsumptions( const std::string& strParentTagName, xml::xistream& xis );
    virtual ~PHY_DotationConsumptions();

    //! @name Accessors
    //@{
    void AddConsumptionValues( T_ConsumptionValue& ) const;
    //@}

    //! @name Operations
    //@{
    bool RegisterConsumptionReservations( PHY_DotationGroupContainer& container, bool isTransported ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_DotationCategory*, PHY_DotationConsumption* > T_DotationConsumptionMap;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadDotation( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    T_DotationConsumptionMap dotationConsumptions_;
    //@}
};

#endif // __PHY_DotationConsumptions_h_
