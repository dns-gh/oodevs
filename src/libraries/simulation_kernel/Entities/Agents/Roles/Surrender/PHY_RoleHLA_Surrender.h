// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Surrender/PHY_RoleHLA_Surrender.h $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 2 $
// $Workfile: PHY_RoleHLA_Surrender.h $
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Surrender_h_
#define __PHY_RoleHLA_Surrender_h_

#include "PHY_RoleInterface_Surrender.h"

namespace surrender
{

// =============================================================================
// @class  PHY_RoleHLA_Surrender
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleHLA_Surrender : public PHY_RoleInterface_Surrender
{
public:
             PHY_RoleHLA_Surrender();
    virtual ~PHY_RoleHLA_Surrender();

    //! @name Main
    //@{
    virtual bool Capture( const MIL_AgentPion& pionTakingPrisoner );
    virtual bool Release();
    virtual bool Imprison( const MIL_Object_ABC& camp );
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_Army_ABC* GetArmySurrenderedTo() const;
    virtual       bool          IsSurrendered       () const;
    virtual       bool          IsPrisoner          () const;
    virtual       bool          IsImprisoned        ( const MIL_Object_ABC& camp );
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}



    //! @name Operations
    //@{
    virtual void NotifySurrendered      () {};
    virtual void NotifySurrenderCanceled() {};

    virtual void Update    ( bool /*bIsDead*/ ) {};
    virtual void Clean     () {};
    virtual bool HasChanged() const { return true; };
    //@}


    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& /*msg*/ ) const {};
    virtual void SendFullState   ( client::UnitAttributes& /*msg*/ ) const {};
    //@}

private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const uint )
    {
        ar & boost::serialization::base_object< surrender::PHY_RoleInterface_Surrender >( *this );
        ar & bPrisoner_;
    }
    //@}

    bool bPrisoner_;
};

} // namespace surrender

BOOST_CLASS_EXPORT_KEY( surrender::PHY_RoleHLA_Surrender )

#endif // __PHY_RoleHLA_Surrender_h_
