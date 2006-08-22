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
    std::string CreateSymbol       ( const AgentNature& nature ) const;
    std::string CreateLevelSymbol  ( const AgentNature& nature ) const;
    std::string CreateAutomatSymbol() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< SymbolRule* >   T_Rules;
    typedef T_Rules::const_iterator    CIT_Rules;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SymbolFactory( const SymbolFactory& );            //!< Copy constructor
    SymbolFactory& operator=( const SymbolFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadRule( xml::xistream& xis, T_Rules& rules ) const;
    void ReadAutomatRule( xml::xistream& xis );
    std::string CreateSymbolFromRules( const AgentNature& nature, const T_Rules& rules ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_Rules symbolRules_;
    T_Rules levelRules_;
    std::string automatSymbol_;
    //@}
};

}

#endif // __SymbolFactory_h_
