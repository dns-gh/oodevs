// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Ghost_h_
#define __Ghost_h_

#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class GhostPrototype;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace xml
{
    class xistream;
}

class IdManager;

// =============================================================================
/** @class  Ghost
    @brief  Ghost
*/
// Created: ABR 2011-10-14
// =============================================================================
class Ghost : public kernel::EntityImplementation< kernel::Ghost_ABC >
            , public kernel::Extension_ABC
            , public kernel::Drawable_ABC
            , public kernel::Serializable_ABC
            , public kernel::Displayable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Ghost( kernel::Controller& controller, IdManager& idManager, const kernel::GhostPrototype& prototype );
             Ghost( kernel::Controller& controller, IdManager& idManager, xml::xistream& xis );
    virtual ~Ghost();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void Rename( const QString& name );
    //@}

    //! @name Accessors
    //@{
    virtual E_GhostType GetGhostType() const;
    virtual const std::string& GetSymbol() const;
    virtual const std::string& GetLevel() const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    //@}

private:
    //! @name Member data
    //@{
    E_GhostType ghostType_;
    QString     type_;
    std::string symbol_;
    std::string level_;
    //@}
};

#endif // __Ghost_h_
