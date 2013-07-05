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

#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
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

// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup : public gui::EntityImplementation< kernel::KnowledgeGroup_ABC >
                     , public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< sword::KnowledgeGroupUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( unsigned long nId, const std::string& name, kernel::Controller& controller, const std::string& sType, const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operation
    //@{
    virtual void DoUpdate( const sword::KnowledgeGroupUpdate& message );
    //@}

    //! @name Accessors
    //@{
    virtual bool IsActivated() const;
    const std::string& GetType() const;
    //@}

private:

    //! @name Accessors
    //@{
    void CreateDictionary();
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
    //@}
};

#endif // __KnowledgeGroup_h_
