// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h $
// $Author: Jvt $
// $Modtime: 31/03/05 10:10 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Refugee.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Refugee_h_
#define __PHY_RoleInterface_Refugee_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/vector.hpp>

namespace client
{
    class UnitAttributes;
}

class MIL_Object_ABC;

// =============================================================================
// @class  PHY_RoleInterface_Refugee
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Refugee : public tools::Role_ABC
                                , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Refugee RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Refugee() {}
    virtual ~PHY_RoleInterface_Refugee() {}
    //@}

    //! @name Accessors
    //@{
    virtual bool IsManaged() const = 0;
    virtual bool IsManaged( const MIL_Object_ABC& camp ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;
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

#endif // __PHY_RoleInterface_Refugee_h_
