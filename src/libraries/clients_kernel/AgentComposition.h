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

#include "tools/Resolver_ABC.h"
#include <boost/noncopyable.hpp>
#include <vector>

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
class AgentComposition : private boost::noncopyable
{
    typedef std::vector< std::pair< std::string, unsigned int > > T_Groups;

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
    unsigned int GetCrew() const;
    const T_Groups& GetGroups() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadGroup( xml::xistream& xis );
    //@}

    //! @name Member data
    //@{
    const ComponentType& type_;
    const unsigned int count_;
    const unsigned int crew_;
    T_Groups groups_;
    //@}
};

}

#endif // __AgentComposition_h_
