// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SymbolFactory_h_
#define __SymbolFactory_h_

#include <boost/noncopyable.hpp>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class AgentNature;
    class SymbolRule;
    class HierarchyLevel_ABC;

// =============================================================================
/** @class  SymbolFactory
    @brief  Symbol factory
*/
// Created: SBO 2006-03-20
// =============================================================================
class SymbolFactory : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SymbolFactory();
    explicit SymbolFactory( xml::xistream& xis );
    virtual ~SymbolFactory();
    //@}

    //! @name Operations
    //@{
    std::string CreateSymbol( const std::string& hierarchy ) const;
    std::string CreateLevelSymbol( const std::string& level ) const;
    std::string CreateLevelSymbol( const kernel::HierarchyLevel_ABC& level ) const;
    std::string CreateAutomatSymbol() const;
    SymbolRule* GetSymbolRule() const;
    bool IsThisChainAvailable( const std::string& chain ) const;
    void Load( const tools::ExerciseConfig& config );
    const std::vector< std::string >& GetAvailableSymbols() const;
    void FillSymbols( const std::string& symbol, const std::string& karma, std::set< std::string >& result ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadSymbols( xml::xistream& xis );
    void ListSymbols();
    void TraverseTree( xml::xostream& xos, const SymbolRule& rule );
    SymbolRule* ReadRule( xml::xistream& xis, const std::string& ruleName, std::string& base ) const;
    void ReadRule( xml::xistream& xis, SymbolRule*& rule ) const;
    //@}

private:
    //! @name Member data
    //@{
    bool initialized_;
    std::string symbolBase_;
    std::auto_ptr< SymbolRule > symbolRule_;
    std::string levelBase_;
    std::auto_ptr< SymbolRule > levelRule_;
    std::string automatSymbol_;
    std::vector< std::string* > currentChain_;
    std::vector< std::string* > currentSymbol_;
    std::vector< std::string > availableSymbols_;
    //@}
};

}

#endif // __SymbolFactory_h_
