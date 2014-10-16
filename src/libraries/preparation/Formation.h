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

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/Formation_ABC.h"
#include "ENT/ENT_Enums.h"

namespace kernel
{
    class Controller;
}

namespace gui
{
    class GLView_ABC;
    class Viewport_ABC;
}

namespace xml
{
    class xistream;
}

namespace tools
{
    class IdManager;
}

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: SBO 2006-09-19
// =============================================================================
class Formation : public gui::EntityImplementation< kernel::Formation_ABC >
                , public kernel::Extension_ABC
                , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( kernel::Controller& controller, E_NatureLevel level, tools::IdManager& idManager );
             Formation( xml::xistream& xis, kernel::Controller& controller, tools::IdManager& idManager );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual QString GetBasicName() const;
    virtual E_NatureLevel GetLevel() const;
    virtual void Rename( const QString& name );
    void SetLevel( E_NatureLevel level );
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void InitializeSymbol() const;
    //@}

private:
    //! @name Member data
    //@{
    E_NatureLevel level_;
    mutable std::string symbolPath_;
    mutable std::string levelPath_;
    //@}
};

#endif // __Formation_h_
