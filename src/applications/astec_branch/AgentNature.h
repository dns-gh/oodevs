// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentNature_h_
#define __AgentNature_h_

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  AgentNature
    @brief  Agent nature
*/
// Created: SBO 2006-03-20
// =============================================================================
class AgentNature
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentNature( xml::xistream& xis );
    virtual ~AgentNature();
    //@}

    //! @name Operations
    //@{
    const std::string& Retrieve( const std::string& symbolName ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentNature( const AgentNature& );            //!< Copy constructor
    AgentNature& operator=( const AgentNature& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadNature( xml::xistream& xis, const std::string& name, const std::string& symbolName );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string >  T_Natures;
    typedef T_Natures::const_iterator           CIT_Natures;
    //@}

private:
    //! @name Member data
    //@{
    T_Natures natures_;
    //@}
};

#endif // __AgentNature_h_
