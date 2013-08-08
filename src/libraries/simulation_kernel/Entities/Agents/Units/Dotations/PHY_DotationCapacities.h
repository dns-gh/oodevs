// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCapacities.h $
// $Author: Nld $
// $Modtime: 26/01/05 14:43 $
// $Revision: 2 $
// $Workfile: PHY_DotationCapacities.h $
//
// *****************************************************************************

#ifndef __PHY_DotationCapacities_h_
#define __PHY_DotationCapacities_h_

#include <tools/Map.h>

namespace xml
{
    class xistream;
}

class PHY_DotationType;
class PHY_DotationCategory;
class PHY_DotationCapacity;
class PHY_DotationGroupContainer;

// =============================================================================
// @class  PHY_DotationCapacities
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCapacities : private boost::noncopyable
{
private:
    typedef std::map< const PHY_DotationCategory*, double > T_Dotations;

public:
             PHY_DotationCapacities( const std::string& strParentTagName, xml::xistream& xis );
    virtual ~PHY_DotationCapacities();

    //! @name Operations
    //@{
    void RegisterCapacities( PHY_DotationGroupContainer& container, T_Dotations* dotations ) const;
    T_Dotations UnregisterCapacities( PHY_DotationGroupContainer& container ) const;
    double GetDefaultLowThreshold( const PHY_DotationCategory& category ) const;
    double GetDefaultHighThreshold( const PHY_DotationCategory& category ) const;
    double GetCapacity( const PHY_DotationCategory& category ) const;
    double GetNormalizedConsumption( const PHY_DotationCategory& category ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_DotationCategory*, PHY_DotationCapacity* > T_DotationCapacityMap;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadDotationCategories( xml::xistream& xis, const PHY_DotationType& dotationType );
    void ReadCategory( xml::xistream& xis );
    void ReadDotation( xml::xistream& xis, const PHY_DotationType& dotationType );
    //@}

private:
    T_DotationCapacityMap dotationCapacities_;
};

#endif // __PHY_DotationCapacities_h_
