// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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

namespace MsgsSimToClient
{
    class MsgKnowledgeGroupUpdate;
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
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgKnowledgeGroupUpdate > // LTO
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( unsigned long nId, kernel::Controller& controller, const std::string& sType, const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operation
    //@{
    // LTO begin
    virtual void DoUpdate( const MsgsSimToClient::MsgKnowledgeGroupUpdate& message );
    //@}

    //! @name Accessors
    //@{
    virtual bool IsActivated() const;
    std::string GetType() const;
    // LTO end
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroup( const KnowledgeGroup& );
    KnowledgeGroup& operator=( const KnowledgeGroup& );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types_;
    std::string type_;    
    bool activated_;
    //@}
    // LTO end
};

#endif // __KnowledgeGroup_h_
