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
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& object );
    virtual void Instanciate( Object& object ) const;
    virtual void ProcessAgentInside( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( Object& object, MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy
    //@{
    ProtectionCapacity( const ProtectionCapacity& from );
    //@}

    //! @name Types
    //@{
    typedef std::set< const MIL_Agent_ABC* >    T_AgentContainer;
    typedef T_AgentContainer::iterator          IT_AgentContainer;
    typedef T_AgentContainer::const_iterator    CIT_AgentContainer;
    //@}

private:
    //! @name Member data
    //@{
    int size_max_;
    T_AgentContainer container_;
    bool bGeniePrepared_;
    //@}
};

#endif