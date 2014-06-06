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
#include <map>
#include <set>

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
    class HierarchyLevel_ABC;
    class Karma;
    class SymbolRule;

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
    std::string CreateMoveSymbol( const std::string& hierarchy ) const;
    std::string CreateStaticSymbol( const std::string& hierarchy ) const;
    std::string GetNatureFromSymbol( const std::string& symbol ) const;
    std::string CreateLevelSymbol( const std::string& level ) const;
    std::string CreateLevelSymbol( const kernel::HierarchyLevel_ABC& level ) const;
    std::string CreateAutomatSymbol() const;
    std::string GetSymbolBase( const Karma& karma ) const;
    SymbolRule* GetSymbolRule() const;
    void Load( const tools::ExerciseConfig& config );
    void Unload();
    const std::vector< std::string >& GetAvailableSymbols() const;
    void FillSymbols( const std::string& symbol, const std::string& karma, std::set< std::string >& result ) const;
    void Merge( const std::string& from, std::string& to ) const;
    std::string GetAssociatedStatic( const std::string& symbol ) const;
    std::string GetAssociatedMove( const std::string& symbol ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadSymbols( xml::xistream& xis );
    void ListSymbols();
    void TraverseTree( const SymbolRule& rule );
    void FillSymbols( const SymbolRule& rule, std::vector< std::string* >& currentChain, std::vector< std::string* >& currentSymbol );
    SymbolRule* ReadRule( xml::xistream& xis, const std::string& ruleName, std::string& base ) const;
    void ReadRule( xml::xistream& xis, SymbolRule*& rule ) const;
    //@}

private:
    //! @name Member data
    //@{
    bool initialized_;
    std::string symbolBase_;
    size_t karmaPosition_;
    std::unique_ptr< SymbolRule > symbolRule_;
    std::string levelBase_;
    std::unique_ptr< SymbolRule > levelRule_;
    std::string automatSymbol_;
    std::vector< std::string > availableSymbols_;
    std::map< std::string, std::string > symbolNatureMap_;
    //@}
};

}

#endif // __SymbolFactory_h_
