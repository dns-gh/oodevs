// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "ASN_Types.h"

namespace kernel
{
    class Controller;
    class KnowledgeGroup_ABC;
}

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat : public kernel::Automat_ABC
              , public kernel::Extension_ABC
              , public kernel::Drawable_ABC
              , public kernel::Updatable_ABC< ASN1T_MsgChangeGroupeConnaissanceAck >
{

public:
    //! @name Constructors/Destructor
    //@{
             Automat( const ASN1T_MsgAutomateCreation& message, kernel::Controller& controller, 
                      const kernel::Resolver_ABC< kernel::AutomatType >& resolver, 
                      const kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;

    virtual const kernel::AutomatType& GetType() const;
    virtual       kernel::KnowledgeGroup_ABC& GetKnowledgeGroup() const;

    virtual void AddAgent( unsigned long id, kernel::Agent_ABC& agent );
    virtual void RemoveAgent( unsigned long id );
    //@}

    //! @name Extensions
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DoUpdate( const ASN1T_MsgChangeGroupeConnaissanceAck& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ChangeKnowledgeGroup( kernel::KnowledgeGroup_ABC* group );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::AutomatType& type_;
    const kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver_;

    kernel::KnowledgeGroup_ABC* group_;
    QString name_;
    unsigned long id_;
    //@}
};

#endif // __Automat_h_
