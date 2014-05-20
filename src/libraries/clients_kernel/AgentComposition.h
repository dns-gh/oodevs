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

#include <tools/Resolver_ABC.h>
#include <boost/noncopyable.hpp>

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
    bool IsMajor() const;
    bool IsConvoyer() const;
    //@}

private:
    //! @name Member data
    //@{
    const ComponentType& type_;
    const unsigned int count_;
    const unsigned int crew_;
    const bool major_;
    const bool convoyer_;
    //@}
};

}

#endif // __AgentComposition_h_
