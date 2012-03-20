// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatType_h_
#define __AutomatType_h_

#include "tools/Resolver_ABC.h"

namespace xml { class xistream; };

namespace tools
{
        template< typename Container > class Iterator;
}

namespace kernel
{
    class DecisionalModel;
    class AgentType;
    class AutomatComposition;

// =============================================================================
/** @class  AutomatType
    @brief  AutomatType
*/
// Created: AGE 2006-02-14
// =============================================================================
class AutomatType
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatType( xml::xistream& xis, const  tools::Resolver_ABC< AgentType, std::string >& agentResolver
                                            , const  tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver );
    virtual ~AutomatType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    std::string GetName() const;
    const AgentType* GetTypePC() const;
    std::string GetSymbol() const;
    std::string GetTypeName() const;
    const DecisionalModel& GetDecisionalModel() const;
    tools::Iterator< const AutomatComposition& > CreateIterator() const;

    bool HasLogistics() const;
    bool IsTC2() const;
    bool IsLogisticSupply() const;
    bool IsLogisticMaintenance() const;
    bool IsLogisticMedical() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatType( const AutomatType& );            //!< Copy constructor
    AutomatType& operator=( const AutomatType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAgent( xml::xistream& xis, const  tools::Resolver_ABC< AgentType, std::string >& agentResolver );
    void UpdateSymbol();
    //@}

    //! @name Types
    //@{
    typedef std::vector< AutomatComposition* > T_UnitConstitution;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    std::string type_;
    std::string symbol_;

    DecisionalModel* model_;
    const AgentType* pcType_;
    T_UnitConstitution units_;
    //@}
};

}

#endif // __AutomatType_h_
