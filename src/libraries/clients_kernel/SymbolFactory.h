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

namespace xml {
    class xistream;
    class xofstream;
}

namespace kernel
{
    class AgentNature;
    class SymbolRule;

// =============================================================================
/** @class  SymbolFactory
    @brief  Symbol factory
*/
// Created: SBO 2006-03-20
// =============================================================================
class SymbolFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             SymbolFactory();
    virtual ~SymbolFactory();
    //@}

    //! @name Operations
    //@{
    std::string CreateSymbol       ( const std::string& hierarchy ) const;
    std::string CreateLevelSymbol  ( const std::string& level ) const;
    std::string CreateAutomatSymbol() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SymbolFactory( const SymbolFactory& );            //!< Copy constructor
    SymbolFactory& operator=( const SymbolFactory& ); //!< Assignment operator
    void ListSymbols();
    void TraverseTree( xml::xofstream& xos, const SymbolRule& rule );
    //@}

    //! @name Helpers
    //@{
    SymbolRule* ReadRule( xml::xistream& xis, const std::string& ruleName, std::string& base ) const;
    void        ReadRule( xml::xistream& xis, SymbolRule*& rule ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string symbolBase_;
    std::auto_ptr< SymbolRule > symbolRule_;
    std::string levelBase_;
    std::auto_ptr< SymbolRule > levelRule_;
    std::string automatSymbol_;
    std::vector< std::string* > currentChain_;
    std::vector< std::string* > currentSymbol_;
    //@}
};

}

#endif // __SymbolFactory_h_
