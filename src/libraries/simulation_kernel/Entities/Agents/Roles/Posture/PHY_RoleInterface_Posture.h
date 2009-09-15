// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h $
// $Author: Jvt $
// $Modtime: 11/05/05 15:17 $
// $Revision: 5 $
// $Workfile: PHY_RoleInterface_Posture.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Posture_h_
#define __PHY_RoleInterface_Posture_h_

#include "MT_Tools/Role_ABC.h"

class PHY_Posture;
class MIL_AgentPion;
class NET_ASN_MsgUnitAttributes;
class HLA_UpdateFunctor;

// =============================================================================
// @class  PHY_RoleInterface_Posture
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Posture : public tools::Role_ABC
                                , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_Posture RoleInterface;

public:
             PHY_RoleInterface_Posture();
    virtual ~PHY_RoleInterface_Posture();

    //! @name Accessors
    //@{
    virtual const PHY_Posture& GetLastPosture                () const = 0;
    virtual const PHY_Posture& GetCurrentPosture             () const = 0;
    virtual       MT_Float     GetPostureCompletionPercentage() const = 0;    
    virtual       bool         CanBePerceived                ( const MIL_AgentPion& perceiver ) const = 0;

    virtual       void         SetPosturePostePrepareGenie   () = 0;
    virtual       void         UnsetPosturePostePrepareGenie () = 0;
    virtual       void         SetTimingFactor               ( MT_Float rFactor ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Update    ( bool bIsDead ) = 0;
    virtual void Clean     () = 0;
    virtual bool HasChanged() const = 0;

    // Override automatic postures
    virtual void SetPostureMovement           () = 0;
    virtual void UnsetPostureMovement         () = 0;

    virtual void EnableDiscreteMode () = 0;
    virtual void DisableDiscreteMode() = 0;
    //@}

    //! @name Installation
    //@{
    virtual bool IsInstalled  () const = 0;
    virtual bool IsUninstalled() const = 0;
    virtual void Install      () = 0;
    //@}

    //! @name Perception
    //@{
    virtual void SetStealthFactor( MT_Float rValue ) = 0;
    //@}

    //! @name Elongation
    //@{
    virtual void     SetElongationFactor( MT_Float ) = 0;
    virtual MT_Float GetElongationFactor() const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual bool IsStealth() const = 0;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& asnMsg ) const = 0;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& asnMsg ) const = 0;
    //@}

    //! @name HLA
    //@{
    virtual void Serialize( HLA_UpdateFunctor& functor ) const = 0;
    virtual bool HLAStatusHasChanged() const = 0;

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

#endif // __PHY_RoleInterface_Posture_h_
