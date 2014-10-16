// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticBaseStates_h__
#define __LogisticBaseStates_h__

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityHierarchies.h"
#include "clients_gui/LogisticHierarchiesBase.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/SubTypes.h"

#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class DotationType;
    class Controller;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

class Dotation;

// =============================================================================
/** @class  LogisticBaseStates
    @brief  LogisticBaseStates
*/
// Created: AHC 2010-09-29
// =============================================================================
class LogisticBaseStates : public gui::EntityHierarchies< gui::LogisticHierarchiesBase >
                         , public kernel::Serializable_ABC
                         , public tools::Resolver< Dotation >
                         , public gui::Drawable_ABC
                         , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticBaseStates( kernel::Controller& controller, kernel::Entity_ABC& entity,
                                 const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver,
                                 gui::PropertiesDictionary& dico, bool canHaveQuotas = true );
    virtual ~LogisticBaseStates();
    //@}

    //! @name Operations
    //@{
    virtual void SetLogisticSuperior( const kernel::LogisticBaseSuperior& superior );
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void ClearDotations();
    virtual void SetDotation( const kernel::DotationType& type, unsigned int quantity );
    void SerializeQuotas( xml::xostream& xos ) const;

    bool CleanBadSubordinates();
    bool HasQuotas() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void SetSuperiorInternal( kernel::Entity_ABC* superior );

    void Load( xml::xistream& xis, const kernel::Entity_ABC* superior );
    void ReadDotation( xml::xistream& xis );

    void SerializeLogistics( xml::xostream& xos ) const;

    void DrawLink( const geometry::Point2f& where, const gui::GLView_ABC& tools, float curve, bool displayLinks, bool displayMissings ) const;
    bool HasMissingLogisticLinks() const;
    virtual void UnregisterCurrentSuperior( const gui::LogisticHierarchiesBase* parent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC& entity_;
    const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver_;
    kernel::LogisticBaseSuperior superior_;
    const bool canHaveQuotas_;
    //@}
};

#endif // __LogisticBaseStates_h__
