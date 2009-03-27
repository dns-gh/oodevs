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
#include "DEC_Knowledge_ObjectAttributeUpdatable_ABC.h"

class SupplyRouteAttribute;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeSupplyRoute 
    : public DEC_Knowledge_ObjectAttribute_ABC
    , private DEC_Knowledge_ObjectAttributeUpdatable_ABC
{
    MT_COPYNOTALLOWED( DEC_Knowledge_ObjectAttributeSupplyRoute ) 

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
    virtual void Send( ASN1T_ObjectAttributes& asnMsg ) const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    void Register( DEC_Knowledge_Object& knObject );
    //@}
    
    //! @name 
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
    bool     bEquipped_;
    MT_Float rWeightSupported_;
    MT_Float rWidth_;
    MT_Float rLength_;
    MT_Float rFlow_;
    //@}
};

#endif // __DEC_Knowledge_ObjectAttributeSupplyRoute_h_
