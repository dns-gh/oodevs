// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SymbolRule_h_
#define __SymbolRule_h_

namespace xml { class xistream; }

namespace kernel
{
    class SymbolCase;
    class SymbolVisitor_ABC;

// =============================================================================
/** @class  SymbolRule
    @brief  Symbol rule
*/
// Created: SBO 2006-03-20
// =============================================================================
class SymbolRule
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SymbolRule( xml::xistream& xis );
    virtual ~SymbolRule();
    //@}

    //! @name Operations
    //@{
    void Evaluate( const std::string& request, std::string& result ) const;
    void Accept( SymbolVisitor_ABC& visitor ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< std::string, SymbolCase* >   T_Cases;
    typedef T_Cases::const_iterator              CIT_Cases;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SymbolRule( const SymbolRule& );            //!< Copy constructor
    SymbolRule& operator=( const SymbolRule& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void          ReadCase( xml::xistream& xis );
    //@}

public:
    //! @name Getters
    //@{
    const SymbolRule::T_Cases& GetCases() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    T_Cases     cases_;
    //@}
};

}

#endif // __SymbolRule_h_
