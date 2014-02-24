// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TemplateElement_ABC_h_
#define __TemplateElement_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

class ColorController;

// =============================================================================
/** @class  TemplateElement_ABC
    @brief  TemplateElement_ABC
*/
// Created: AGE 2007-05-29
// =============================================================================
class TemplateElement_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TemplateElement_ABC() {}
    virtual ~TemplateElement_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual kernel::Entity_ABC* Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f& center, ColorController& colorController ) = 0;
    virtual void Serialize( xml::xostream& output ) = 0;
    virtual bool IsCompatible( const kernel::Entity_ABC& superior ) const = 0;
    virtual QString GetName() const = 0;
    virtual void Rename( const QString& name ) = 0;
    //@}
};

#endif // __TemplateElement_ABC_h_
