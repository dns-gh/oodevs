// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __AgentComposition_h_
#define __AgentComposition_h_

#include "Tools/Resolver_ABC.h"

namespace xml { class xistream; };

namespace kernel
{
    class ComponentType;

// =============================================================================
/** @class  AgentComposition
    @brief  AgentComposition
*/
// Created: SBO 2010-03-23
// =============================================================================
class AgentComposition
{
public:
    //! @name Constructors/Destructor
    //@{
            AgentComposition( xml::xistream& xis, const tools::Resolver_ABC< ComponentType, std::string >& resolver );
    virtual ~AgentComposition();
    //@}

    //! @name Operations
    //@{
    const ComponentType& GetType() const;
    unsigned int GetCount() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentComposition( const AgentComposition& );            //!< Copy constructor
    AgentComposition& operator=( const AgentComposition& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const ComponentType& type_;
    const unsigned int count_;
    //@}
};

}

#endif // __AgentComposition_h_
