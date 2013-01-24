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

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class PropertiesDictionary;
    class LogisticLevel;
    class StaticModel;
    class AutomatType;
}

namespace sword
{
    class AutomatCreation;
}

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat : public gui::EntityImplementation< kernel::Automat_ABC >
              , public kernel::Extension_ABC
              , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( const sword::AutomatCreation& message, kernel::Controller& controller, const kernel::AutomatType& type );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AutomatType& GetType() const;
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;
    //@}

    //! @name Extensions
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( const kernel::AutomatType& type );
    void InitializeSymbol() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AutomatType& type_;
    mutable std::string symbol_; // $$$$ RC LDC: code duplication with preparation
    mutable std::string level_;
    const kernel::LogisticLevel* logisticLevel_;
    //@}
};

#endif // __Automat_h_
