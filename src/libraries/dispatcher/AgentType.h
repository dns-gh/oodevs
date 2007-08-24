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

namespace xml { class xistream; };

namespace kernel
{    
    class AgentNature;    
    class SymbolFactory;    

// =============================================================================
/** @class  AgentType
    @brief  AgentType
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentType
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentType( xml::xistream& xis, const SymbolFactory& symbolFactory );
    virtual ~AgentType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    const std::string& GetName() const;    
    const AgentNature& GetNature() const;

    const std::string& GetSymbol() const;
    const std::string& GetLevelSymbol() const;
    const std::string& GetHQSymbol() const;    
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentType( const AgentType& );            //!< Copy constructor
    AgentType& operator=( const AgentType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::string     name_;
    std::string     type_;
    unsigned long   id_;
    std::string     model_;
    
    AgentNature* nature_;
    std::string symbol_; // $$$$ AGE 2006-10-24: devrait etre dans nature
    std::string levelSymbol_;
    std::string hqSymbol_;
    //@}
};

}

#endif // __AgentType_h_
