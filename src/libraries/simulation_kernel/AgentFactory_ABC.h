// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AgentFactory_ABC_h_
#define __AgentFactory_ABC_h_

#include <boost/noncopyable.hpp>

class MIL_AgentPion;
class MIL_AgentTypePion;
class MIL_Automate;
class MT_Vector2D;

namespace sword
{
    class RoleExtender_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  AgentFactory_ABC
    @brief  AgentFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class AgentFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory_ABC() {}
    virtual ~AgentFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis, sword::RoleExtender_ABC* ext ) = 0;
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, sword::RoleExtender_ABC* ext ) = 0;
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name, sword::RoleExtender_ABC* ext ) = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& , const unsigned int ) {}
    //@}
};

#endif // __AgentFactory_ABC_h_
