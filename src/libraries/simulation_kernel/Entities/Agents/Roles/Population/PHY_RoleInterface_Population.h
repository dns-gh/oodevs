// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RoleInterface_Population.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:31 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_Population.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Population_h_
#define __PHY_RoleInterface_Population_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>

class MIL_Population;

// =============================================================================
// @class  PHY_RoleInterface_Population
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Population : public tools::Role_ABC
                                   , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Population RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Population() {}
    virtual ~PHY_RoleInterface_Population() {}
    //@}

    //! @name Accessors
    //@{
    virtual bool IsInvulnerable() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;
    virtual bool HasChanged() const = 0;
    virtual double GetCollidingPopulationDensity() const = 0;
    virtual bool HasCollision() const = 0;
    virtual bool HasCollisionWithCrowd( const MIL_Population& population ) const = 0;
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

#endif // __PHY_RoleInterface_Population_h_
