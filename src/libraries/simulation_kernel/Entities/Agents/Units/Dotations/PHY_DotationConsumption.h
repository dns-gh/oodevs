// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationConsumption.h $
// $Author: Jvt $
// $Modtime: 7/02/05 11:20 $
// $Revision: 2 $
// $Workfile: PHY_DotationConsumption.h $
//
// *****************************************************************************

#ifndef __PHY_DotationConsumption_h_
#define __PHY_DotationConsumption_h_

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;

// =============================================================================
// @class  PHY_DotationConsumption
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationConsumption : private boost::noncopyable
{
public:
             PHY_DotationConsumption( const PHY_DotationCategory& category, xml::xistream& xis );
    virtual ~PHY_DotationConsumption();

    //! @name Accessors
    //@{
    double GetConsumption() const;
    //@}

private:
    //! @name Member data
    //@{
    const PHY_DotationCategory& category_;
    double rConsumption_;
    //@}
};

#endif // __PHY_DotationConsumption_h_
