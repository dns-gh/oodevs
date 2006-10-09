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
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"

namespace kernel
{
    class AutomatType;
    class AgentTypes;
    class Controller;
    class KnowledgeGroup_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

class IdManager;

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat : public kernel::Automat_ABC
              , public kernel::Extension_ABC
              , public kernel::Drawable_ABC
              , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Automat( const kernel::AutomatType& type, kernel::Controller& controller, IdManager& idManager );
             Automat( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, const kernel::AgentTypes& agentTypes );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;

    virtual const kernel::AutomatType& GetType() const;
    //@}

    //! @name Extensions
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoSerialize( xml::xostream& xos ) const;
    void CreateDictionary();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::AutomatType* type_;
    QString name_;
    unsigned long id_;
    //@}
};

#endif // __Automat_h_
