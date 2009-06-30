// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h $
// $Author: Jvt $
// $Modtime: 30/03/05 17:09 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_HumanFactors.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_HumanFactors_h_
#define __PHY_RolePion_HumanFactors_h_

#include "MIL.h"
#include "PHY_RoleInterface_HumanFactors.h"

namespace xml
{
    class xistream;
}

class NET_ASN_MsgUnitAttributes;
class PHY_Morale;
class PHY_Experience;
class PHY_Tiredness;

// =============================================================================
// @class  PHY_RolePion_HumanFactors
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_HumanFactors : public PHY_RoleInterface_HumanFactors
{
    MT_COPYNOTALLOWED( PHY_RolePion_HumanFactors )

public:
             PHY_RolePion_HumanFactors( MT_RoleContainer& role );
             PHY_RolePion_HumanFactors();
    virtual ~PHY_RolePion_HumanFactors();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    //@}

    //! @name Main
    //@{
    void ReadOverloading( xml::xistream& xis );
    void SetMorale      ( const PHY_Morale& morale         );
    void SetExperience  ( const PHY_Experience& experience );
    void SetTiredness   ( const PHY_Tiredness&  tiredness  );
    //@}

    //! @name Accessors
    //@{
    const PHY_Morale&     GetMorale    () const;
    const PHY_Experience& GetExperience() const;
    const PHY_Tiredness&  GetTiredness () const;
    //@}
            
     //! @name Operations
     //@{
    MT_Float ModifyMaxSpeed              ( MT_Float rSpeed    ) const;
    MT_Float ModifyReloadingDuration     ( MT_Float rDuration ) const;
    MT_Float ModifyPH                    ( MT_Float rPH       ) const;
    MT_Float ModifyPostureTime           ( MT_Float rTime     ) const;
    MT_Float GetSensorDistanceModificator() const;
     //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

private: 
    bool bHasChanged_;

    const PHY_Morale*     pMorale_;
    const PHY_Experience* pExperience_;
    const PHY_Tiredness*  pTiredness_;

private:
    //! @name Helpers
    //@{
    void ReadFacteursHumains( xml::xistream& xis );
    void ReadFatigue        ( xml::xistream& xis );
    void ReadMoral          ( xml::xistream& xis );
    void ReadExperience     ( xml::xistream& xis );
    //@}
};

#endif // __PHY_RolePion_HumanFactors_h_
