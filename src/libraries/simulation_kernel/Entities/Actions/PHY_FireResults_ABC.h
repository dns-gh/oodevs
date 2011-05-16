// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PHY_FireResults_ABC.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_FireResults_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_FireResults_ABC_h_
#define __PHY_FireResults_ABC_h_

#include "PHY_FireDamages_Agent.h"
#include "PHY_FireDamages_Population.h"
#include "Tools/MIL_IDManager.h"

namespace sword
{
    class CrowdsFireDamages;
    class UnitsFireDamages;
}

class MIL_Agent_ABC;
class MIL_Population;

// =============================================================================
// @class  PHY_FireResults_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_FireResults_ABC : private boost::noncopyable
{
public:
             PHY_FireResults_ABC();
    virtual ~PHY_FireResults_ABC();

    //! @name Accessors
    //@{
    PHY_FireDamages_Agent&      GetDamages( const MIL_Agent_ABC&  target );
    PHY_FireDamages_Population& GetDamages( const MIL_Population& target );
    //@}

    //! @name Refs
    //@{
    void IncRef();
    void DecRef();
    //@}

    //! @name Refs
    //@{
    virtual void Hit();
    //@}

protected:
    //! @name Types
    //@{
    typedef std::map< const MIL_Agent_ABC*, PHY_FireDamages_Agent > T_AgentDamagesMap;
    typedef T_AgentDamagesMap::const_iterator                       CIT_AgentDamagesMap;

    typedef std::map< const MIL_Population*, PHY_FireDamages_Population > T_PopulationDamagesMap;
    typedef T_PopulationDamagesMap::const_iterator                        CIT_PopulationDamagesMap;
    //@}

protected:
    //! @name Accessors $$$ A VIRER
    //@{
    const T_PopulationDamagesMap& GetPopulationDamages() const;
    //@}

    //! @name Network tools
    //@{
    void Serialize( sword::UnitsFireDamages& asn ) const;
    void Serialize( sword::CrowdsFireDamages& asn ) const;
    void SendDamagesPion( const MIL_Agent_ABC& firer, unsigned int fireId, bool direct ) const;
    void SendDamagesCrowd( const MIL_Population& firer, unsigned int fireId ) const;
    static MIL_IDManager idManager_;
    //@}

private:
    //! @name Member data
    //@{
    T_AgentDamagesMap      agentsDamages_;
    T_PopulationDamagesMap populationsDamages_;
    unsigned int           nNbrRefs_;
    //@}
};

#endif // __PHY_FireResults_ABC_h_
