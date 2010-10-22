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
    class LogisticLevel;
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
                     const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const kernel::LogisticLevel& currentLevel,
                     kernel::PropertiesDictionary& dictionary );
    virtual ~LogisticLinks();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;

    kernel::Automat_ABC* GetTC2() const;
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
    void DrawLink( const geometry::Point2f& from, kernel::Entity_ABC* to, const kernel::GlTools_ABC& tools, float curve, bool link, bool missing ) const;
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    const kernel::LogisticLevel& currentLevel_;

    kernel::Automat_ABC*   tc2_;
    kernel::Entity_ABC*    superior_;
    //@}
};

#endif // __LogisticLinks_h_
