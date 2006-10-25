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

namespace kernel
{

// =============================================================================
/** @class  AgentNature
    @brief  Agent_ABC nature
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
    const std::string& GetLevel() const;
    const std::string& GetNature() const;
    const std::string& GetAtlas() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentNature( const AgentNature& );            //!< Copy constructor
    AgentNature& operator=( const AgentNature& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::string level_;
    std::string nature_;
    std::string atlas_;
    //@}
};

}

#endif // __AgentNature_h_
