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


namespace MsgsSimToClient
{
    class MsgCrowdFireDamages;
}

class MIL_Population;
struct FireResult;

// =============================================================================
// @class  PHY_FireDamages_Population
// Created: JVT 2004-08-03
// =============================================================================
class PHY_FireDamages_Population
{
public:
             PHY_FireDamages_Population();
    virtual ~PHY_FireDamages_Population();

    //! @name Notifications
    //@{
    void NotifyHumansKilled( unsigned int nNbrHumans );
    //@}

    //! @name Accessors
    //@{
    unsigned int GetNbrKilledHumans() const;
    //@}

    //! @name Network
    //@{
    void Serialize( const MIL_Population& target, MsgsSimToClient::MsgCrowdFireDamages& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nNbrKilledHumans_;
    //@}
};

#endif // __PHY_FireDamages_Population_h_
