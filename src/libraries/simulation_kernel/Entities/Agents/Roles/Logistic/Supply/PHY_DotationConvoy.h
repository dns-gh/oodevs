// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_DotationConvoy.h $
// $Author: Jvt $
// $Modtime: 11/04/05 17:23 $
// $Revision: 2 $
// $Workfile: PHY_DotationConvoy.h $
//
// *****************************************************************************

#ifndef __PHY_DotationConvoy_h_
#define __PHY_DotationConvoy_h_

#include "PHY_Convoy_ABC.h"

namespace xml
{
    class xistream;
}

class PHY_SupplyDotationConsign;

// =============================================================================
// @class  PHY_DotationConvoy
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationConvoy : public PHY_Convoy_ABC
{
    MT_COPYNOTALLOWED( PHY_DotationConvoy )

public:
    //! @name 
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    //@}

public:
    explicit PHY_DotationConvoy( PHY_SupplyDotationConsign& consign );
             PHY_DotationConvoy();
    virtual ~PHY_DotationConvoy();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint ); 
    //@}

    //! @name Operations
    //@{    
    uint GetTravelTimeToLoadingPoint  () const;
    uint GetTravelTimeToUnloadingPoint() const;
    uint GetTravelTimeToFormingPoint  () const;
    //@}

private:
    //! @name Tools
    //@{
    uint GetTravelTime( const MT_Vector2D& startPos, const MT_Vector2D& endPos ) const;
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_DotationConvoy::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_Convoy_ABC >( *this );
}

#endif // __PHY_DotationConvoy_h_
