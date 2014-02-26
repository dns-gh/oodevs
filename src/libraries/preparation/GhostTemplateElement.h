// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef GhostTemplateElement_h
#define GhostTemplateElement_h

#include "TemplateElement.h"

enum E_GhostType;

namespace kernel
{
    class Ghost_ABC;
    class GhostPrototype;
}

class GhostModel;

// =============================================================================
/** @class  GhostTemplateElement
    @brief  GhostTemplateElement
*/
// Created: ABR 2014-02-24
// =============================================================================
class GhostTemplateElement : public TemplateElement
{
public:
    //! @name Constructors/Destructor
    //@{
             GhostTemplateElement( GhostModel& ghosts,
                                   const kernel::Ghost_ABC& ghost );
             GhostTemplateElement( GhostModel& ghosts,
                                   xml::xistream& xis );
             GhostTemplateElement( GhostModel& ghosts,
                                   E_GhostType ghostType,
                                   xml::xistream& xis );
    virtual ~GhostTemplateElement();
    //@}

private:
    //! @name TemplateElement implementation
    //@{
    virtual kernel::Entity_ABC* Instanciate( kernel::Entity_ABC& superior,
                                             const geometry::Point2f& center,
                                             ColorController& colorController );
    virtual void Serialize( xml::xostream& xos ) const;
    virtual bool IsCompatible( const kernel::Entity_ABC& superior ) const;
    //@}

private:
    //! @name Member data
    //@{
    GhostModel& ghosts_;
    std::unique_ptr< kernel::GhostPrototype > ghost_;
    const bool isCommandPost_;
    const bool isLogisticBase_;
    const bool fromUnknownType_;
    //@}
};

#endif // GhostTemplateElement_h
