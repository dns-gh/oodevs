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
#include "tools/Resolver_ABC.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class KnowledgeGroupType;
}

class IdManager;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup : public kernel::EntityImplementation< kernel::KnowledgeGroup_ABC >
                     , public kernel::Extension_ABC
                     , public kernel::Serializable_ABC
                     , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( kernel::Controller& controller, IdManager& idManager, tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types );
             KnowledgeGroup( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operations
    //@{
    void Rename( const QString& name );
    void SetType( kernel::KnowledgeGroupType* const& );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual bool IsActivated() const;
    void CheckType( std::string& loadingErrors ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    void UpdateCommunicationDelay();
    kernel::KnowledgeGroupType* ResolveType( const std::string& typeName, tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types );
    //@}

private:
    //! @name Member data
    //@{
    kernel::KnowledgeGroupType* type_;
    std::string communicationDelay_;
    //@}
};

#endif // __KnowledgeGroup_h_
