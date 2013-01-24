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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/EntityImplementation.h"

namespace kernel
{
    class Controller;
    class AutomatType;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace tools
{
    class IdManager;
}

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat : public kernel::EntityImplementation< kernel::Automat_ABC >
              , public kernel::Extension_ABC
              , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( const kernel::AutomatType& type, kernel::Controller& controller, tools::IdManager& idManager, const QString& name );
             Automat( xml::xistream& xis, kernel::Controller& controller, tools::IdManager& idManager, const kernel::AutomatType& type );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;
    const kernel::AutomatType& GetType() const;
    //@}

    //! @name Extensions
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeSymbol() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AutomatType& type_;
    mutable std::string symbol_;
    mutable std::string level_;
    //@}
};

#endif // __Automat_h_
