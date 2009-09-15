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

class MIL_NbcAgentType;
class NET_ASN_MsgUnitAttributes;
class PHY_Morale;
class PHY_Experience;
class PHY_Tiredness;

// =============================================================================
// @class  PHY_RoleInterface_HumanFactors
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_HumanFactors : public tools::Role_ABC
                                     , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_HumanFactors RoleInterface;

public:
             PHY_RoleInterface_HumanFactors();
    virtual ~PHY_RoleInterface_HumanFactors();

public:
    //! @name Operations
    //@{
    virtual void Update    ( bool bIsDead ) = 0;
    virtual void Clean     () = 0;
    virtual bool HasChanged() const = 0;
    //@}

    //! @name Main
    //@{
    virtual void ReadOverloading( xml::xistream& xis ) = 0;
    virtual void SetMorale      ( const PHY_Morale& morale         ) = 0;
    virtual void SetExperience  ( const PHY_Experience& experience ) = 0;
    virtual void SetTiredness   ( const PHY_Tiredness&  tiredness  ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_Morale&     GetMorale    () const = 0;
    virtual const PHY_Tiredness&  GetTiredness () const = 0;
    //@}

    //! @name Operations
    //@{
    virtual MT_Float ModifyMaxSpeed              ( MT_Float rSpeed    ) const = 0;
    virtual MT_Float ModifyReloadingDuration     ( MT_Float rDuration ) const = 0;
    virtual MT_Float ModifyPH                    ( MT_Float rPH       ) const = 0;
    virtual MT_Float ModifyPostureTime           ( MT_Float rTime     ) const = 0;
    virtual MT_Float GetSensorDistanceModificator() const = 0;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const = 0;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const = 0;
    //@}
private:
    //! @name Serialization
	//@{
	friend class boost::serialization::access;
	template< typename Archive > void serialize( Archive& ar, const uint )
	{
		ar & boost::serialization::base_object< tools::Role_ABC >( *this );
	}
	//@}
};
#endif // __PHY_RoleInterface_HumanFactors_h_
