// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h $
// $Author: Jvt $
// $Modtime: 30/03/05 17:05 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_HumanFactors.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_HumanFactors_h_
#define __PHY_RoleInterface_HumanFactors_h_

#include "MT_Tools/Role_ABC.h"

namespace xml
{
    class xistream;
}

namespace client
{
    class UnitAttributes;
}

class PHY_Morale;
class PHY_Experience;
class PHY_Tiredness;
class PHY_Stress;

// =============================================================================
// @class  PHY_RoleInterface_HumanFactors
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_HumanFactors : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_HumanFactors RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_HumanFactors() {}
    virtual ~PHY_RoleInterface_HumanFactors() {}
    //@}

public:
    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;
    virtual bool HasChanged() const = 0;
    //@}

    //! @name Main
    //@{
    virtual void ReadOverloading( xml::xistream& xis ) = 0;
    virtual void SetMorale( const PHY_Morale& morale ) = 0;
    virtual void SetExperience( const PHY_Experience& experience ) = 0;
    virtual void SetTiredness( const PHY_Tiredness& tiredness, bool updateValue = false ) = 0;
    virtual void SetStress( const PHY_Stress& stress, bool updateValue = false ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_Morale& GetMorale() const = 0;
    virtual const PHY_Tiredness& GetTiredness () const = 0;
    //@}

    //! @name Operations
    //@{
    virtual double ModifyPH ( double rPH ) const = 0;
    virtual void NotifyAttacked() = 0;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const = 0;
    virtual void SendFullState( client::UnitAttributes& msg ) const = 0;
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
#endif // __PHY_RoleInterface_HumanFactors_h_
