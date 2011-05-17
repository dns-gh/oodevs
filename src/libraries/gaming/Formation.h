// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Drawable_ABC.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class FormationCreation;
    class FormationUpdate;
    class LogisticLevel;
}

namespace kernel
{
    class StaticModel;
}

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: AGE 2006-10-19
// =============================================================================
class Formation : public kernel::EntityImplementation< kernel::Formation_ABC >
                , public kernel::Updatable_ABC< sword::FormationUpdate >
                , public kernel::Extension_ABC
                , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( const sword::FormationCreation& message, kernel::Controller& controller, const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& resolver );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    void InitializeSymbol() const;
    bool IsAggregated( const kernel::Entity_ABC& entity ) const;
    bool HasAggregatedSubordinate() const;
    void DoUpdate( const sword::FormationUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller&               controller_;
    const kernel::HierarchyLevel_ABC& level_;
    const kernel::LogisticLevel*      logisticLevel_;
    mutable std::string               symbolPath_; // $$$$ RC LDC: code duplication with preparation
    mutable std::string               levelPath_;
    //@}
};

#endif // __Formation_h_
