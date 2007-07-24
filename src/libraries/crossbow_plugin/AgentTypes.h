// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentTypes_h_
#define __AgentTypes_h_

#include "Resolver.h"

namespace dispatcher
{
    class Config;
}


namespace xml { class xistream; };

namespace kernel
{
    class AgentType;    
    class AutomatType;          
    class SymbolFactory;
        
// =============================================================================
/** @class  AgentTypes
    @brief  AgentTypes
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentTypes : public Resolver< AgentType >
                 , public Resolver< AgentType, std::string >
                 , public Resolver< AutomatType >
                 , public Resolver< AutomatType, std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentTypes( const dispatcher::Config& config );
    virtual ~AgentTypes();
    //@}

    //! @name Operations
    //@{
    void Purge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentTypes( const AgentTypes& );            //!< Copy constructor
    AgentTypes& operator=( const AgentTypes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{    
    void Load( const dispatcher::Config& config );
    void ReadAgents( const std::string& agents );
    void ReadAutomats( const std::string& automats );    

    void ReadAgentType( xml::xistream& xis );
    void ReadAutomatType( xml::xistream& xis );    
    //@}

private:
    //! @name Member data
    //@{
    SymbolFactory* symbolFactory_;
    //@}
};

}

#endif // __AgentTypes_h_
