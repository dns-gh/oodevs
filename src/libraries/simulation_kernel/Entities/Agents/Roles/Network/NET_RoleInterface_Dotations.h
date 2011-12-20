// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Network/NET_RoleInterface_Dotations.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:38 $
// $Revision: 2 $
// $Workfile: NET_RoleInterface_Dotations.h $
//
// *****************************************************************************

#ifndef __NET_RoleInterface_Dotations_h_
#define __NET_RoleInterface_Dotations_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>

// =============================================================================
// @class  NET_RoleInterface_Dotations
// Created: JVT 2004-08-03
// =============================================================================
class NET_RoleInterface_Dotations : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef NET_RoleInterface_Dotations RoleInterface;
    //@}

public:
             NET_RoleInterface_Dotations();
    virtual ~NET_RoleInterface_Dotations();

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

BOOST_CLASS_EXPORT_KEY( NET_RoleInterface_Dotations )

#endif // __NET_RoleInterface_Dotations_h_
