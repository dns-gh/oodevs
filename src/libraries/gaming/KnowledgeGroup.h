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

#include "ASN_Types.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
    class InstanciationComplete;
}

// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup : public kernel::KnowledgeGroup_ABC
                     , public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< kernel::InstanciationComplete >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( unsigned long nId, kernel::Controller& controller );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Accessors & Modifiers
    //@{
    virtual unsigned long GetId() const;
    virtual QString GetName() const;
    virtual void DoUpdate( const kernel::InstanciationComplete& );
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
    kernel::Controller& controller_;
    unsigned long      nID_;
    QString name_;
    //@}
};

#endif // __KnowledgeGroup_h_
