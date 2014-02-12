// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_InteractiveContainer_ABC_h_
#define __MIL_InteractiveContainer_ABC_h_

class MIL_Agent_ABC;
class MIL_Object_ABC;
class MIL_PopulationElement_ABC;

// =============================================================================
/** @class  MIL_InteractiveContainer_ABC
    @brief  MIL_InteractiveContainer_ABC
*/
// Created: JCR 2008-05-30
// =============================================================================
class MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_InteractiveContainer_ABC() {}
    virtual ~MIL_InteractiveContainer_ABC() {}
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}

    //! @name Operations on agent
    //@{
    virtual void CanInteractWith         ( const MIL_Object_ABC& /*object*/, const MIL_Agent_ABC& /*agent*/, bool& /*canInteract*/ ) {}
    virtual void PreprocessAgent         ( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ) {}
    virtual void ProcessAgentEntering    ( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ) {}
    virtual void ProcessAgentExiting     ( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ) {}
    virtual void ProcessAgentMovingInside( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ) {}
    virtual void ProcessAgentInside      ( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ) {}
    //@}

    //! @name Operations on population
    //@{
    virtual void ProcessPopulationInside( MIL_Object_ABC& /*object*/, MIL_PopulationElement_ABC& /*population*/ ) {}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_InteractiveContainer_ABC& operator=( const MIL_InteractiveContainer_ABC& ); //!< Assignment operator
    //@}
};

#endif // __MIL_InteractiveContainer_ABC_h_
