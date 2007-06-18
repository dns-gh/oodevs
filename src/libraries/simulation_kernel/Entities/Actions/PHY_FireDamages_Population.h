// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PHY_FireDamages_Population.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 5 $
// $Workfile: PHY_FireDamages_Population.h $
//
// *****************************************************************************

#ifndef __PHY_FireDamages_Population_h_
#define __PHY_FireDamages_Population_h_

#include "MIL.h"

class MIL_Population;

struct ASN1T_FireResult;

// =============================================================================
// @class  PHY_FireDamages_Population
// Created: JVT 2004-08-03
// =============================================================================
class PHY_FireDamages_Population
{
public:
     PHY_FireDamages_Population();
    ~PHY_FireDamages_Population();

    //! @name Notifications
    //@{
    void NotifyHumansKilled( uint nNbrHumans );
    //@}

    //! @name Accessors
    //@{
    uint GetNbrKilledHumans() const;
    //@}

    //! @name Network
    //@{
           void Serialize              ( const MIL_Population& target, ASN1T_PopulationFireDamages& asn ) const;
    static void CleanAfterSerialization( ASN1T_PopulationFireDamages& asn );
    //@}

private:
    uint nNbrKilledHumans_;
};

#include "PHY_FireDamages_Population.inl"

#endif // __PHY_FireDamages_Population_h_
