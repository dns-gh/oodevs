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

#include <boost/noncopyable.hpp>

enum E_StockCategory;

namespace xml { class xistream; };

namespace tools
{
    template< typename Container > class Iterator;
    template< typename T, typename Identifier > class Resolver_ABC;
}

namespace kernel
{
    class AgentComposition;
    class AgentNature;
    class ComponentType;
    class DecisionalModel;
    class DotationCapacityType;
    class SymbolFactory;

// =============================================================================
/** @class  AgentType
    @brief  AgentType
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentType : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentType( xml::xistream& xis, const tools::Resolver_ABC< ComponentType, std::string >& componentResolver
                                          , const tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver
                                          , const SymbolFactory& symbolFactory );
    virtual ~AgentType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    std::string GetName() const;
    const DecisionalModel& GetDecisionalModel() const;
    const AgentNature& GetNature() const;
    tools::Iterator< const AgentComposition& > CreateIterator() const;
    tools::Iterator< const DotationCapacityType& > CreateResourcesIterator() const;
    unsigned int GetComponentCount( const ComponentType& ) const;

    const std::string& GetSymbol() const;
    const std::string& GetLevelSymbol() const;
    const std::string& GetHQSymbol() const;

    unsigned int GetNbrOfficers() const;
    unsigned int GetNbrWarrantOfficers() const;

    bool IsTC2() const;
    bool IsLogisticSupply() const;
    bool IsLogisticMaintenance() const;
    bool IsLogisticMedical() const;
    bool HasStocks() const;

    bool IsStockCategoryDefined( E_StockCategory category ) const;
    unsigned int GetStockCategoryThreshold( E_StockCategory category ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< AgentComposition* >      T_Components;
    typedef T_Components::const_iterator        CIT_Components;
    typedef std::vector< DotationCapacityType* >  T_Resources;
    typedef T_Resources::const_iterator         CIT_Resources;
    typedef std::map< E_StockCategory, unsigned int > T_StocksThresholds;
    typedef T_StocksThresholds::const_iterator  CIT_StocksThresholds;
    //@}

    //! @name Helpers
    //@{
    void ReadEquipment( xml::xistream& xis, const  tools::Resolver_ABC< ComponentType, std::string >& resolver  );
    void ReadCrewRank( xml::xistream& xis );
    void ReadResourcesCategory( xml::xistream& xis );
    void ReadResources( xml::xistream& xis );
    void ReadStock( xml::xistream& xis );
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

    unsigned int nbrOfficers_;
    unsigned int nbrWarrantOfficers_;
    T_Resources resources_;

    T_StocksThresholds stocks_;
    //@}
};

}

#endif // __AgentType_h_
