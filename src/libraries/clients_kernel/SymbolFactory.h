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

namespace xml { class xistream; }

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
    explicit SymbolFactory( xml::xistream& xis );
    virtual ~SymbolFactory();
    //@}

    //! @name Operations
    //@{
    std::string CreateSymbol       ( const std::string& hierarchy ) const;
    std::string CreateLevelSymbol  ( const std::string& level ) const;
    std::string CreateAutomatSymbol() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SymbolFactory( const SymbolFactory& );            //!< Copy constructor
    SymbolFactory& operator=( const SymbolFactory& ); //!< Assignement operator
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
    SymbolRule* symbolRule_;
    std::string levelBase_;
    SymbolRule* levelRule_;
    std::string automatSymbol_;
    //@}
};

}

#endif // __SymbolFactory_h_
