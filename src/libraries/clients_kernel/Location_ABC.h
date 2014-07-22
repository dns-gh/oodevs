// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Location_ABC_h_
#define __Location_ABC_h_

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#include <QtGui/qfont.h>
#pragma warning( pop )

#include <geometry/types.h>
#include "Types.h"

namespace kernel
{
    class LocationVisitor_ABC;

// =============================================================================
/** @class  Location_ABC
    @brief  Location definition
*/
// Created: AGE 2006-08-09
// =============================================================================
class Location_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Location_ABC() {}
    virtual ~Location_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void PopPoint() = 0;
    virtual void AddPoint( const geometry::Point2f& point ) = 0;
    virtual void AddText( const QString& text, const QFont& font ) = 0;
    virtual void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision ) = 0;
    virtual Location_ABC& Clone() const = 0;
    virtual std::string GetTypeName() const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual void Accept( LocationVisitor_ABC& ) const = 0;
    virtual QString GetName() const = 0;
    virtual bool IsValid() const = 0;
    virtual bool IsDone() const = 0;
    //@}
};

}

#endif // __Location_ABC_h_
