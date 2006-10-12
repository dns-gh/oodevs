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

namespace xml
{
    class xistream;
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
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( kernel::Controller& controller, IdManager& idManager );
             KnowledgeGroup( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operations
    //@{
    void Rename( const QString& name );
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroup( const KnowledgeGroup& );
    KnowledgeGroup& operator=( const KnowledgeGroup& );
    //@}

    //! @name Helpers
    //@{
    static unsigned long ReadId  ( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    QString type_;
    //@}
};

#endif // __KnowledgeGroup_h_
