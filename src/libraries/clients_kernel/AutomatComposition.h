// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatComposition_h_
#define __AutomatComposition_h_

#include <boost/noncopyable.hpp>

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
class AutomatComposition : public boost::noncopyable
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
    //! @name Member data
    //@{
    const AgentType& type_;
    unsigned int min_;
    unsigned int max_;
    //@}
};

}

#endif // __AutomatComposition_h_
