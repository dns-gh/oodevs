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

class ProtectionCapacity 
    : public ObjectCapacity_ABC
    , private MIL_InteractiveContainer_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
	explicit ProtectionCapacity( xml::xistream& xis );
             ProtectionCapacity();
	virtual ~ProtectionCapacity();
	//@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& /*object*/ );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;
	//@}

    //! @name MIL_InteractiveContainer_ABC
    //@{
    virtual void ProcessAgentInside( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( Object& object, MIL_Agent_ABC& agent );
    //@}

private:
    //! @name 
    //@{
    typedef std::set< const MIL_Agent_ABC* >    T_AgentContainer;
    typedef T_AgentContainer::iterator          IT_AgentContainer;
    typedef T_AgentContainer::const_iterator    CIT_AgentContainer;
    //@}

private:
    //! @name Copy
    //@{
    ProtectionCapacity( const ProtectionCapacity& from );
    //@}

private:
    //! @name 
    //@{
    int size_max_;
    T_AgentContainer container_;
    bool bGeniePrepared_;
    //@}
};

#endif