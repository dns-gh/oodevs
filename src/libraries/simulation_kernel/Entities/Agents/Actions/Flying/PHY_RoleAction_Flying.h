// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Flying/PHY_RoleAction_Flying.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 7 $
// $Workfile: PHY_RoleAction_Flying.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_Flying_h_
#define __PHY_RoleAction_Flying_h_

#include "MIL.h"
#include "PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Effects/MIL_Effect_Fly.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"

class PHY_ActionFly;
class MIL_Agent_ABC;

namespace location
{
    class LocationComputer_ABC;
}

namespace posture
{
    class PostureComputer_ABC;
}

namespace moving
{
    class MoveComputer_ABC;
}

// =============================================================================
// @class  PHY_RoleAction_Flying
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Flying : public PHY_RoleAction_InterfaceFlying
                            , public tools::AlgorithmModifier_ABC< location::LocationComputer_ABC >
                            , public tools::AlgorithmModifier_ABC< posture::PostureComputer_ABC >
                            , public tools::AlgorithmModifier_ABC< moving::MoveComputer_ABC >
{
public:
    explicit PHY_RoleAction_Flying( MIL_Agent_ABC& entity );
    virtual ~PHY_RoleAction_Flying();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean();
    virtual bool CanMove() const;
    //@}

    //! @name Action interface
    //@{
    virtual void Fly();
    virtual void SetFlyingHeight( double rHeight );
    virtual bool IsFlying() const;
    //@}

    //! @name Effect interface
    //@{
    virtual void Apply( double rHeight );
    //@}

private:
    //! @name Tools
    //@{
    bool TakeOff();
    bool Land();
    virtual void Execute( location::LocationComputer_ABC& algorithm ) const;
    virtual void Execute( posture::PostureComputer_ABC& algorithm ) const;
    virtual void Execute( moving::MoveComputer_ABC& algorithm ) const;
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RoleAction_Flying )
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC&                     owner_;
    MIL_Effect_Fly                     effectFly_;
    boost::shared_ptr< PHY_ActionFly > pActionFly_;
    double                             rHeight_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RoleAction_Flying )
INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RoleAction_Flying, MIL_Agent_ABC )

#endif // __PHY_RoleAction_Flying_h_
