// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatType_h_
#define __AutomatType_h_

#include "tools/Resolver_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

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
    class SymbolFactory;

// =============================================================================
/** @class  AutomatType
    @brief  AutomatType
*/
// Created: AGE 2006-02-14
// =============================================================================
class AutomatType : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatType( xml::xistream& xis, const tools::Resolver_ABC< AgentType, std::string >& agentResolver
                                            , const tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver
                                            , const SymbolFactory& symbolFactory );
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
    unsigned int NumberOfAgents() const;

    bool HasLogistics() const;
    bool IsTC2() const;
    bool IsLogisticSupply() const;
    bool IsLogisticMaintenance() const;
    bool IsLogisticMedical() const;
    //@}

    //! @name Methods
    //@{
    typedef boost::ptr_vector< AutomatComposition > T_Compositions;
    const T_Compositions& GetCompositions() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadAgent( xml::xistream& xis, const  tools::Resolver_ABC< AgentType, std::string >& agentResolver );
    void UpdateSymbol( const SymbolFactory& symbolFactory );
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
    T_Compositions compositions_;
    //@}
};

}

#endif // __AutomatType_h_
