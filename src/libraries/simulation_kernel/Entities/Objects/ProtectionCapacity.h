// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProtectionCapacity_h_
#define __ProtectionCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ProtectionCapacity
    @brief  ProtectionCapacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class ProtectionCapacity : public ObjectCapacity_ABC
                         , private MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ProtectionCapacity();
    explicit ProtectionCapacity( xml::xistream& xis );
    virtual ~ProtectionCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy
    //@{
    ProtectionCapacity( const ProtectionCapacity& from );
    //@}

    //! @name Types
    //@{
    typedef std::set< const MIL_Agent_ABC* >   T_AgentContainer;
    typedef T_AgentContainer::iterator        IT_AgentContainer;
    typedef T_AgentContainer::const_iterator CIT_AgentContainer;
    //@}

private:
    //! @name Member data
    //@{
    int size_max_;
    T_AgentContainer container_;
    bool bGeniePrepared_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ProtectionCapacity )

#endif
