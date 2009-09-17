// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AgentFactory_h_
#define __AgentFactory_h_

#include "AgentFactory_ABC.h"
#include "clients_kernel/Resolver.h"

class MIL_IDManager;
class AlgorithmsFactories;

// =============================================================================
/** @class  AgentFactory
    @brief  AgentFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class AgentFactory : public AgentFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory( MIL_IDManager& idManager );
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis );
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentFactory( const AgentFactory& );            //!< Copy constructor
    AgentFactory& operator=( const AgentFactory& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    void Initialize( MIL_AgentPion& pion, MIL_Automate& automate, const MT_Vector2D& vPosition );
    //@}

private:
    //! @name Member data
    //@{
    MIL_IDManager& idManager_;
    std::auto_ptr< AlgorithmsFactories > algorithmsFactories_;
    //@}
};

#endif // __AgentFactory_h_
