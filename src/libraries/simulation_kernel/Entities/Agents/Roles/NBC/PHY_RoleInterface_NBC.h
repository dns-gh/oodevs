// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RoleInterface_NBC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:31 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_NBC.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_NBC_h_
#define __PHY_RoleInterface_NBC_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>

namespace client
{
    class UnitAttributes;
}

class MIL_ToxicEffectManipulator;
class MIL_DisasterType;

namespace nbc
{
// =============================================================================
// @class  PHY_RoleInterface_NBC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_NBC : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_NBC RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_NBC() {}
    virtual ~PHY_RoleInterface_NBC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsContaminated() const = 0;
    virtual bool IsIntoxicated() const = 0;
    virtual void Poison( const MIL_ToxicEffectManipulator& contamination ) = 0;
    virtual void Contaminate( const MIL_ToxicEffectManipulator& contamination ) = 0;
    virtual void Afflict( float dose, const MIL_DisasterType& type ) = 0;
    virtual void Decontaminate() = 0;
    virtual void Decontaminate( double rRatioAgentsWorking ) = 0;
    virtual double GetContaminationQuantity() const = 0;
    virtual bool IsForcedImmune() const = 0;
    virtual void TemporaryImmunizeAgent( bool bImmunize ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;
    virtual bool HasChanged() const = 0;
    //@}

    //! @name Main
    //@{
    virtual void WearNbcProtectionSuit() = 0;
    virtual void RemoveNbcProtectionSuit() = 0;

    //@}

private:
    //! @name Serialization
    //@{
    friend class ::boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & ::boost::serialization::base_object< ::tools::Role_ABC >( *this );
    }
    //@}
};

}

#endif // __PHY_RoleInterface_NBC_h_
