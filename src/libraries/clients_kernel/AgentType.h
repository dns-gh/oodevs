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

#include "tools/Resolver_ABC.h"

namespace xml { class xistream; };

namespace tools
{
    template< typename Container > class Iterator;
}

namespace kernel
{
    class ComponentType;
    class AgentNature;
    class DecisionalModel;
    class SymbolFactory;

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
             AgentType( xml::xistream& xis, const  tools::Resolver_ABC< ComponentType, std::string >& componentResolver
                                          , const  tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver
                                          , const SymbolFactory& symbolFactory );
    virtual ~AgentType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    std::string GetName() const;
    const DecisionalModel& GetDecisionalModel() const;
    const AgentNature& GetNature() const;
    tools::Iterator< const ComponentType& > CreateIterator() const;
    unsigned int GetComponentCount( const ComponentType& ) const;

    const std::string& GetSymbol() const;
    const std::string& GetLevelSymbol() const;
    const std::string& GetHQSymbol() const;

    bool IsTC2() const;
    bool IsLogisticSupply() const;
    bool IsLogisticMaintenance() const;
    bool IsLogisticMedical() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentType( const AgentType& );            //!< Copy constructor
    AgentType& operator=( const AgentType& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const ComponentType*, unsigned int > T_Components;
    //@}

    //! @name Helpers
    //@{
    void ReadEquipment( xml::xistream& xis, const  tools::Resolver_ABC< ComponentType, std::string >& resolver  );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string type_;
    unsigned long id_;

    DecisionalModel* model_;
    AgentNature* nature_;
    T_Components equipments_;
    std::string symbol_; // $$$$ AGE 2006-10-24: devrait etre dans nature
    std::string levelSymbol_;
    std::string hqSymbol_;
    //@}
};

}

#endif // __AgentType_h_
