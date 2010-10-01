// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectBouchonMines.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:10 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectBouchonMines.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeSupplyRoute_h_
#define __DEC_Knowledge_ObjectAttributeSupplyRoute_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include <boost/serialization/export.hpp>

class SupplyRouteAttribute;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeSupplyRoute : public DEC_Knowledge_ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeSupplyRoute();
    explicit DEC_Knowledge_ObjectAttributeSupplyRoute( const SupplyRouteAttribute& attr );
    virtual ~DEC_Knowledge_ObjectAttributeSupplyRoute();
    //@}

    //! @name Update
    //@{
    virtual void UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel );
    virtual void UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision );
    virtual void Send( Common::ObjectAttributes& asnMsg ) const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    void Register( DEC_Knowledge_Object& knObject );
    //@}

    //! @name Helpers
    //@{
    bool IsEquipped() const;
    //@}

private:
    //! @name Update
    //@{
    void UpdateAttributes( bool bRecognized );
    //@}

private:
    //! @name Data Members
    //@{
    const SupplyRouteAttribute* attr_;
    bool bEquipped_;
    double rWeightSupported_;
    double rWidth_;
    double rLength_;
    double rFlow_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeSupplyRoute )

#endif // __DEC_Knowledge_ObjectAttributeSupplyRoute_h_
