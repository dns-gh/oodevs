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
// LTO begin
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class KnowledgeGroupType;
}

namespace sword
{
    class KnowledgeGroupUpdate;
}
// LTO end

// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup : public kernel::EntityImplementation< kernel::KnowledgeGroup_ABC >
                     , public kernel::Extension_ABC // LTO
                     , public kernel::Updatable_ABC< sword::KnowledgeGroupUpdate > // LTO
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( unsigned long nId, const std::string& name, kernel::Controller& controller, const std::string& sType, const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types, bool isJammed );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operation
    //@{
    // LTO begin
    virtual void DoUpdate( const sword::KnowledgeGroupUpdate& message );
    //@}

    //! @name Accessors
    //@{
    virtual bool IsActivated() const;
    virtual bool IsJammed() const;
    virtual bool IsCrowd() const;
    std::string GetType() const;

    // LTO end
    //@}

private:

    //! @name Accessors
    //@{
    void CreateDictionary( kernel::Controller& controller );
    //@}

    //! @name Copy/Assignment
    //@{
    KnowledgeGroup( const KnowledgeGroup& );
    KnowledgeGroup& operator=( const KnowledgeGroup& );
    //@}

private:
    //! @name Member data
    //@{
    std::string delay_;
    std::string type_;
    const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types_;
    bool activated_;
    bool isJammed_;
    //@}
    // LTO end
};

#endif // __KnowledgeGroup_h_
