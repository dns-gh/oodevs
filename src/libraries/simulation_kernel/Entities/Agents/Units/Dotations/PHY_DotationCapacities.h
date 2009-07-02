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

#include "MIL.h"

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

public:
     PHY_DotationCapacities( const std::string& strParentTagName, xml::xistream& xis );
    ~PHY_DotationCapacities();

    //! @name Operations
    //@{
    void RegisterCapacities  ( PHY_DotationGroupContainer& container ) const;
    void UnregisterCapacities( PHY_DotationGroupContainer& container ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, PHY_DotationCapacity* > T_DotationCapacityMap;
    typedef T_DotationCapacityMap::const_iterator                          CIT_DotationCapacityMap;  
    //@}

private:
    //! @name Init
    //@{
    void ReadDotationCategories( xml::xistream& xis, const PHY_DotationType& dotationType );
    //@}
    //! @name Helpers
    //@{
    void ReadCategory( xml::xistream& xis );
    void ReadDotation( xml::xistream& xis, const PHY_DotationType& dotationType );
    //@}

private:
    T_DotationCapacityMap dotationCapacities_;
};

#include "PHY_DotationCapacities.inl"

#endif // __PHY_DotationCapacities_h_
