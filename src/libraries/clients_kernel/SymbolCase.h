// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SymbolCase_h_
#define __SymbolCase_h_

namespace xml { class xistream; }

namespace kernel
{
    class SymbolRule;
    class SymbolVisitor_ABC;

// =============================================================================
/** @class  SymbolCase
    @brief  Symbol case
*/
// Created: SBO 2006-03-20
// =============================================================================
class SymbolCase
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SymbolCase( xml::xistream& xis );
    virtual ~SymbolCase();
    //@}

    //! @name Operations
    //@{
    void Evaluate( const std::string& request, std::string& result ) const;
    bool EvaluateMove( const std::string& request, std::string& result ) const;
    bool EvaluateStatic( const std::string& request, std::string& result ) const;
    void Accept( SymbolVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SymbolCase( const SymbolCase& );            //!< Copy constructor
    SymbolCase& operator=( const SymbolCase& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadRule( xml::xistream& xis );
    //@}

public:
    //! @name Getters
    //@{
    const std::string& GetValue() const;
    const SymbolRule* GetRule() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string value_;
    std::string move_;
    std::string static_;
    std::unique_ptr< SymbolRule > rule_;
    //@}
};

}

#endif // __SymbolCase_h_
