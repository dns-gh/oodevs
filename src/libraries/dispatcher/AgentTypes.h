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

namespace dispatcher
{
    class Config;
}

namespace xml { class xistream; };

namespace kernel
{
    class AgentType;
    class SymbolFactory;

// =============================================================================
/** @class  AgentTypes
    @brief  AgentTypes
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentTypes
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentTypes( const dispatcher::Config& config );
    virtual ~AgentTypes();
    //@}

    //! @name Operations
    //@{
    const AgentType& Get( unsigned long id ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentTypes( const AgentTypes& );            //!< Copy constructor
    AgentTypes& operator=( const AgentTypes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Purge();
    void Load( const dispatcher::Config& config );
    void ReadAgents( const std::string& agents );
    void ReadAgentType( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, const AgentType* > T_AgentTypes;
    typedef T_AgentTypes::const_iterator              CIT_AgentTypes;
    //@}

private:
    //! @name Member data
    //@{
    T_AgentTypes agentTypes_;
    SymbolFactory* symbolFactory_;
    //@}
};

}

#endif // __AgentTypes_h_
