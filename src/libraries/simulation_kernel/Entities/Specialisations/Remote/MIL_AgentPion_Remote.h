// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AgentPion_Remote_h_
#define __MIL_AgentPion_Remote_h_

#include "Entities/Agents/MIL_AgentPion.h"

class MIL_AgentPion_Remote : public MIL_AgentPion
{
public:
    //! @name Constructors/Destructor
    //@{
    MIL_AgentPion_Remote( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis, const MIL_Config& config );
    MIL_AgentPion_Remote( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string& name );
    virtual ~MIL_AgentPion_Remote();
    //@}
};

#endif // __MIL_AgentPion_Remote_h_
