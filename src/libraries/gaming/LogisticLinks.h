// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticLinks_h_
#define __LogisticLinks_h_

#include "Dotation.h"
#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/Resolver_ABC.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace sword
{
    class ChangeLogisticLinks;
    class ParentEntity;
    class LogSupplyQuotas;
    class SeqOfDotationQuota;
}

namespace kernel
{
    class Controller;
    class Automat_ABC;
    class Formation_ABC;
    class Displayer_ABC;
    class AutomatType;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

class LogisticLink;

// =============================================================================
/** @class  LogisticLinks
    @brief  LogisticLinks
*/
// Created: AGE 2006-02-13
// =============================================================================
class LogisticLinks : public kernel::LogisticLinks_ABC
                    , public kernel::Updatable_ABC< sword::ChangeLogisticLinks >
                    , public kernel::Updatable_ABC< sword::LogSupplyQuotas >
                    , public gui::Drawable_ABC
                    , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticLinks( kernel::Controller& controller,
                            const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                            const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver,
                            const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                            gui::PropertiesDictionary& dictionary,
                            kernel::Entity_ABC& entity );
    virtual ~LogisticLinks();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;

    bool HasSuperior( const kernel::Entity_ABC& entity ) const;
    tools::Iterator< const LogisticLink& > CreateSuperiorLinksIterator() const;

    const kernel::Entity_ABC* GetNominalSuperior() const;
    const kernel::Entity_ABC* GetCurrentSuperior() const;
    const kernel::Entity_ABC& GetEntity() const;

    LogisticLink* FindLogisticLink( const kernel::Entity_ABC& superior ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ChangeLogisticLinks& message );
    virtual void DoUpdate( const sword::LogSupplyQuotas& message );

    kernel::Entity_ABC* FindParentEntity( const sword::ParentEntity& message ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< LogisticLink > > T_SuperiorLinks;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    T_SuperiorLinks superiorLinks_;
    std::vector< const kernel::Entity_ABC* > superiors_;
    kernel::Entity_ABC& entity_;
    const QString property_;
    //@}
};

#endif // __LogisticLinks_h_
