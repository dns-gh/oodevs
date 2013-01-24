// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticBaseStates_h__
#define __LogisticBaseStates_h__

#include "tools/Resolver.h"
#include "LogisticHierarchiesBase.h"
#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/SubTypes.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class DotationType;
    class Controller;
    class Entity_ABC;
    class PropertiesDictionary;
}

class Dotation;
class DotationsItem;

// =============================================================================
/** @class  LogisticBaseStates
    @brief  LogisticBaseStates
*/
// Created: AHC 2010-09-29
// =============================================================================
class LogisticBaseStates : public kernel::EntityHierarchies< LogisticHierarchiesBase >
                         , public kernel::Serializable_ABC
                         , public tools::Resolver< Dotation >
                         , public gui::Drawable_ABC
                         , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticBaseStates( kernel::Controller& controller, kernel::Entity_ABC& entity,
                 const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, kernel::PropertiesDictionary& dico, bool canHaveQuotas = true );
    virtual ~LogisticBaseStates();
    //@}

    //! @name Operations
    //@{
    virtual void SetLogisticSuperior( const kernel::LogisticBaseSuperior& superior );
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void ClearDotations();
    virtual void SetDotation( const kernel::DotationType& type, unsigned int quantity );
    void SerializeQuotas( xml::xostream& xos ) const;

    bool CleanBadSubordinates();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner );
    virtual void SetSuperiorInternal( kernel::Entity_ABC* superior );

    void Load( xml::xistream& xis, const kernel::Entity_ABC* superior );
    void ReadDotation( xml::xistream& xis );

    void SerializeLogistics( xml::xostream& xos ) const;

    void DrawLink( const geometry::Point2f& where, const kernel::GlTools_ABC& tools, float curve, bool displayLinks, bool displayMissings ) const;
    bool IsToSerializeQuotas() const;
    bool HasMissingLogisticLinks() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC& entity_;
    const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver_;
    DotationsItem* item_;
    kernel::LogisticBaseSuperior superior_;
    const bool canHaveQuotas_;
    //@}
};

#endif // __LogisticBaseStates_h__
