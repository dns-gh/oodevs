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

#include "tools/Resolver.h"
#include "Entities/Agents/MIL_AgentPion.h"

class MIL_AgentPion;
class MIL_AgentTypePion;
class MIL_Automate;

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
class AgentFactory_ABC : public tools::Resolver< MIL_AgentPion >
                       , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory_ABC() {}
    virtual ~AgentFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis ) = 0;
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition ) = 0;
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name ) = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentFactory_ABC( const AgentFactory_ABC& );            //!< Copy constructor
    AgentFactory_ABC& operator=( const AgentFactory_ABC& ); //!< Assignment operator
    //@}
};

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void AgentFactory_ABC::serialize
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
template< typename Archive >
void AgentFactory_ABC::serialize( Archive& file, const unsigned int )
{
    file & elements_;
}

#endif // __AgentFactory_ABC_h_
