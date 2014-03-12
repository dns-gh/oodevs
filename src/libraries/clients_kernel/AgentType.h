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
#include <boost/shared_ptr.hpp>
#include "ENT/ENT_Tr.h"

namespace xml
{
    class xistream;
}

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
    class LocalizedString;
    class SymbolFactory;
    class LogisticSupplyClass;
    class XmlTranslations;

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
                                          , const SymbolFactory& symbolFactory, kernel::XmlTranslations& translations );
    virtual ~AgentType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    const std::string& GetLocalizedName() const;
    const std::string& GetKeyName() const;
    const DecisionalModel& GetDecisionalModel() const;
    const AgentNature& GetNature() const;
    tools::Iterator< const AgentComposition& > CreateIterator() const;
    tools::Iterator< const DotationCapacityType& > CreateResourcesIterator() const;
    unsigned int GetComponentCount( const ComponentType& ) const;
    
    const std::string& GetSymbol() const;
    const std::string& GetMoveSymbol() const;
    const std::string& GetStaticSymbol() const;
    const std::string& GetLevelSymbol() const;
    const std::string& GetHQSymbol() const;
    const std::string& GetTypeName() const;
    float GetWidth() const;
    float GetDepth() const;
    float GetLength( bool isSpeedSafety = true ) const;
    const float GetComposantesWeight() const;
    E_AgentNbcSuit GetNbcSuit() const;

    unsigned int GetNbrOfficers() const;
    unsigned int GetNbrWarrantOfficers() const;

    bool IsTC2() const;
    bool IsLogisticSupply() const;
    bool IsLogisticMaintenance() const;
    bool IsLogisticMedical() const;
    bool HasStocks() const;

    bool IsStockCategoryDefined( const LogisticSupplyClass& logClass ) const;
    void GetAllowedSupplyClasses( std::set< std::string >& allowedSupplyClasses ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< AgentComposition* >      T_Components;
    typedef std::vector< DotationCapacityType* >  T_Resources;
    typedef std::map< std::string, unsigned int > T_StocksThresholds;
    //@}

    //! @name Helpers
    //@{
    void ReadEquipment( xml::xistream& xis, const tools::Resolver_ABC< ComponentType, std::string >& resolver  );
    void ReadCrewRank( xml::xistream& xis );
    void ReadResourcesCategory( xml::xistream& xis );
    void ReadResources( xml::xistream& xis );
    void ReadStock( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const boost::shared_ptr< LocalizedString > name_;
    std::string type_;
    unsigned long id_;

    DecisionalModel* model_;
    AgentNature* nature_;
    T_Components equipments_;
    std::string symbol_; // $$$$ AGE 2006-10-24: devrait etre dans nature
    std::string moveSymbol_;
    std::string staticSymbol_;
    std::string levelSymbol_;
    std::string hqSymbol_;
    E_AgentNbcSuit nbcSuit_;

    unsigned int nbrOfficers_;
    unsigned int nbrWarrantOfficers_;
    T_Resources resources_;

    T_StocksThresholds stocks_;
    //@}
};

}

#endif // __AgentType_h_
