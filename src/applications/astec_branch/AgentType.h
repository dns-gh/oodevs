// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentType_h_
#define __AgentType_h_

#include "Resolver_ABC.h"

namespace xml { class xistream; };
class ComponentType;
class Nature;
class DecisionalModel;

// =============================================================================
/** @class  AgentType
    @brief  AgentType
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentType
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentType( xml::xistream& xis, const Resolver_ABC< ComponentType, std::string >& componentResolver
                                          , const Resolver_ABC< DecisionalModel, std::string >& modelResolver );
    virtual ~AgentType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    const std::string& GetName() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentType( const AgentType& );            //!< Copy constructor
    AgentType& operator=( const AgentType& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const ComponentType* > T_Components;
    //@}

    //! @name Helpers
    //@{
    void ReadEquipment( xml::xistream& xis, const Resolver_ABC< ComponentType, std::string >& resolver  );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned long id_;

    DecisionalModel* model_;
    Nature* nature_;
    T_Components equipments_;
    //@}
};

#endif // __AgentType_h_
