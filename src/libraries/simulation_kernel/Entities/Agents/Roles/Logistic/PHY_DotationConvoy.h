// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_DotationConvoy.h $
// $Author: Jvt $
// $Modtime: 11/04/05 17:23 $
// $Revision: 2 $
// $Workfile: PHY_DotationConvoy.h $
//
// *****************************************************************************

#ifndef __PHY_DotationConvoy_h_
#define __PHY_DotationConvoy_h_

#include "MIL.h"
#include "PHY_Convoy_ABC.h"

namespace xml
{
    class xistream;
}

class PHY_SupplyDotationConsign;
class MT_Vector2D;

// =============================================================================
// @class  PHY_DotationConvoy
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationConvoy : public PHY_Convoy_ABC
{
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
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    unsigned int GetTravelTimeToLoadingPoint  () const;
    unsigned int GetTravelTimeToUnloadingPoint() const;
    unsigned int GetTravelTimeToFormingPoint  () const;
    //@}

private:
    //! @name Tools
    //@{
    unsigned int GetTravelTime( const MT_Vector2D& startPos, const MT_Vector2D& endPos ) const;
    //@}

private:
    PHY_SupplyDotationConsign* pConsign_;
};

BOOST_CLASS_EXPORT_KEY( PHY_DotationConvoy )

#endif // __PHY_DotationConvoy_h_
