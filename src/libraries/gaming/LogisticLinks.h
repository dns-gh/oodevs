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

#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace Common
{
    class MsgChangeLogisticLinks;
}

namespace kernel
{
    class Controller;
    class Automat_ABC;
    class Formation_ABC;
    class Displayer_ABC;
    class AutomatType;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  LogisticLinks
    @brief  LogisticLinks
*/
// Created: AGE 2006-02-13
// =============================================================================
class LogisticLinks : public kernel::LogisticLinks_ABC
                    , public kernel::Updatable_ABC< Common::MsgChangeLogisticLinks >
                    , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticLinks( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                     const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, kernel::PropertiesDictionary& dictionary );
    virtual ~LogisticLinks();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;

    kernel::Automat_ABC* GetTC2() const;
    kernel::Automat_ABC* GetAutomatSuperior() const;
    kernel::Formation_ABC* GetFormationSuperior() const;
    kernel::Entity_ABC* GetSuperior() const;

    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticLinks( const LogisticLinks& );            //!< Copy constructor
    LogisticLinks& operator=( const LogisticLinks& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const Common::MsgChangeLogisticLinks& message );
    void DrawLink( const geometry::Point2f& from, kernel::Automat_ABC* to, const kernel::GlTools_ABC& tools, float curve, bool link, bool missing ) const;
    void DrawLink( const geometry::Point2f& from, kernel::Formation_ABC* to, const kernel::GlTools_ABC& tools, float curve, bool link, bool missing ) const;
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;

    kernel::Automat_ABC*   tc2_;
    kernel::Automat_ABC*   automatSuperior_;
    kernel::Formation_ABC* formationSuperior_;
    //@}
};

#endif // __LogisticLinks_h_
