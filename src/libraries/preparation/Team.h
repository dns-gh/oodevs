// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class InstanciationComplete;
}

namespace xml
{
    class xostream;
}

class KnowledgeGroupFactory_ABC;
class IdManager;

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: SBO 2006-08-29
// =============================================================================
class Team : public kernel::Team_ABC
           , public kernel::Extension_ABC
           , public kernel::Updatable_ABC< kernel::InstanciationComplete >
           , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Team( kernel::Controller& controller, KnowledgeGroupFactory_ABC& factory, IdManager& idManager );
    virtual ~Team();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    //@}

    //! @name Operations
    //@{
    void CreateKnowledgeGroup();
    void Rename( const QString& name );
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Team( const Team& );            //!< Copy constructor
    Team& operator=( const Team& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    KnowledgeGroupFactory_ABC& factory_;
    QString name_;
    unsigned long id_;
    //@}
};

#endif // __Team_h_
