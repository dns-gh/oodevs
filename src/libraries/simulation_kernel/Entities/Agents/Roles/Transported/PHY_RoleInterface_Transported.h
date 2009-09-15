// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h $
// $Author: Jvt $
// $Modtime: 31/03/05 11:11 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Transported.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Transported_h_
#define __PHY_RoleInterface_Transported_h_

#include "MIL.h"

#include "MT_Tools/Role_ABC.h"

class MIL_Agent_ABC;
class NET_ASN_MsgUnitAttributes;

// =============================================================================
// @class  PHY_RoleInterface_Transported
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Transported : public tools::Role_ABC
                                    , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_Transported RoleInterface;

public:
             PHY_RoleInterface_Transported();
    virtual ~PHY_RoleInterface_Transported();

    //! @name Operations
    //@{
    virtual bool LoadForTransport   ( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable ) = 0;
    virtual bool UnloadFromTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable ) = 0;
    virtual bool CancelTransport    ( const MIL_Agent_ABC& transporter ) = 0;
    virtual void GetTransportWeight ( bool bTransportOnlyLoadable, MT_Float& rTotalWeight, MT_Float& rHeaviestComposanteWeight ) const = 0 ;
    virtual bool IsTransported      () const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Update    ( bool bIsDead ) = 0;
    virtual void Clean     () = 0;
    virtual bool HasChanged() const = 0;
    //@}

    //! @name Human transporters ... $$$
    //@{
    virtual bool HasHumanTransportersReady    () const = 0;
    virtual void DisableHumanTransporters     ( const MT_Vector2D& vPosition ) = 0;
    virtual void RecoverHumanTransporters     () = 0;
    virtual bool HasHumanTransportersToRecover() const = 0;
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


#endif // __PHY_RoleInterface_Transported_h_
