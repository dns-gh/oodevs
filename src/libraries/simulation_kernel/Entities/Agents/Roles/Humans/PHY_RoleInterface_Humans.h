// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.h $
// $Author: Jvt $
// $Modtime: 31/03/05 16:42 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Humans.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Humans_h_
#define __PHY_RoleInterface_Humans_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

namespace client
{
    class UnitAttributes;
}

class PHY_HumanRank;
class Human_ABC;
class MIL_AutomateLOG;

namespace human
{
// =============================================================================
// @class  PHY_RoleInterface_Humans
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Humans : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Humans RoleInterface;

    enum E_EvacuationMode
    {
        eEvacuationMode_Auto,  // TC2 or IMEX
        eEvacuationMode_Manual // IMEX
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Humans() {}
    virtual ~PHY_RoleInterface_Humans() {}
    //@}

    virtual unsigned int GetNbrLivingHumans() const = 0;
    //@}

    //! @name Medical
    //@{
    virtual void EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const = 0;
    virtual bool HasWoundedHumansToEvacuate() const = 0;
    virtual void ChangeEvacuationMode( E_EvacuationMode nMode ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;

    virtual void HealAllHumans( bool withLog ) = 0;
    virtual void ChangeHumansAvailability( const PHY_HumanRank& rank, unsigned int nNbrAvailable ) = 0;
    virtual unsigned int ReduceHumansAvailability( const PHY_HumanRank& rank, unsigned int reduce ) = 0;
    //@}

private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

} //namespace human

#endif // __PHY_RoleInterface_Humans_h_
