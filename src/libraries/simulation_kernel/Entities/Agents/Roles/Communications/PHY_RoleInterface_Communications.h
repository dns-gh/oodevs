// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h $
// $Author: Jvt $
// $Modtime: 30/03/05 16:49 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_Communications.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Communications_h_
#define __PHY_RoleInterface_Communications_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>

namespace client
{
    class UnitAttributes;
}

class MIL_Object_ABC;
class DEC_Knowledge_ObjectPerception;
class DEC_Knowledge_ObjectCollision;

// =============================================================================
// @class  PHY_RoleInterface_Communications
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Communications : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Communications RoleInterface;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
             PHY_RoleInterface_Communications() {}
    virtual ~PHY_RoleInterface_Communications() {}
    //@}

    //! @name Operations
    //@{
    virtual bool CanEmit() const = 0;
    virtual bool CanReceive() const = 0;
    virtual void Jam( const MIL_Object_ABC& jammer ) = 0;
    virtual void Unjam( const MIL_Object_ABC& jammer ) = 0;
    virtual void ActivateBlackout() = 0;
    virtual void ActivatePartialBlackout( bool report = true ) = 0;
    virtual void DeactivateBlackout() = 0;
    virtual void UpdateKnowledgesFromObjectPerception( const DEC_Knowledge_ObjectPerception& perception ) = 0;
    virtual void UpdateKnowledgesFromObjectCollision( const DEC_Knowledge_ObjectCollision& collision ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;
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

#endif // __PHY_RoleInterface_Communications_h_
