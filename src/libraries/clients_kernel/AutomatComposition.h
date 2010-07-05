// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatComposition_h_
#define __AutomatComposition_h_

namespace xml
{
    class xistream;
}

namespace tools
{
    template< typename T, typename K > class Resolver_ABC;
}

namespace kernel
{
    class AgentType;

// =============================================================================
/** @class  AutomatComposition
    @brief  AutomatComposition // $$$$ AGE 2006-10-03: Le nom est pas terrible
*/
// Created: AGE 2006-09-05
// =============================================================================
class AutomatComposition
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatComposition( xml::xistream& input, const  tools::Resolver_ABC< AgentType, std::string >& agentResolver );
    virtual ~AutomatComposition();
    //@}

    //! @name Accessors
    //@{
    const AgentType& GetType() const;
    unsigned int GetMin() const;
    unsigned int GetMax() const;
    unsigned int GetSensibleNumber() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AutomatComposition( const AutomatComposition& );            //!< Copy constructor
    AutomatComposition& operator=( const AutomatComposition& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const AgentType& type_;
    unsigned int min_;
    unsigned int max_;
    //@}
};

}

#endif // __AutomatComposition_h_
