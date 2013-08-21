// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Observer_ABC.h"
#include "tools/Resolver_ABC.h"

#include <boost/optional.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class KnowledgeGroupType;
    class Controllers;
    class Team_ABC;
}

namespace tools
{
    class IdManager;
}

// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup : public kernel::EntityImplementation< kernel::KnowledgeGroup_ABC >
                     , public kernel::Extension_ABC
                     , public kernel::Serializable_ABC
                     , public tools::Observer_ABC 
                     , public tools::ElementObserver_ABC< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( kernel::Controllers& controllers, tools::IdManager& idManager, tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types, const kernel::Entity_ABC& parent, bool isCrowd );
             KnowledgeGroup( xml::xistream& xis, kernel::Controllers& controllers, tools::IdManager& idManager, tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operations
    //@{
    void SetType( kernel::KnowledgeGroupType* const& );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual bool IsActivated() const;
    virtual bool IsJammed() const;
    virtual bool IsCrowd() const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    void UpdateCommunicationDelay();
    kernel::KnowledgeGroupType* ResolveType( const std::string& typeName, tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    //@}

private:
    //! @name Member data
    //@{
    kernel::KnowledgeGroupType* type_;
    std::string communicationDelay_;
    bool isCrowd_;
    const boost::optional< unsigned int > parentId_;
    kernel::Controllers& controllers_;
    //@}
};

#endif // __KnowledgeGroup_h_
