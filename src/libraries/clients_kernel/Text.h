// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __kernel_Text_h_
#define __kernel_Text_h_

#include "Location_ABC.h"

namespace kernel
{
// =============================================================================
/** @class  Text
    @brief  Text
*/
// Created: LGY 2014-06-11
// =============================================================================
class Text : public Location_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Text();
    virtual ~Text();
    //@}

    //! @name Operations
    //@{
    virtual void PopPoint();
    virtual void AddPoint( const geometry::Point2f& point );
    virtual void AddText( const QString& text, const QFont& font );
    virtual void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    virtual Location_ABC& Clone() const;
    virtual std::string GetTypeName() const;
    //@}

    //! @name Accessors
    //@{
    virtual void Accept( LocationVisitor_ABC& ) const;
    virtual QString GetName() const;
    virtual bool IsValid() const;
    virtual bool IsDone() const;
    //@}

private:
    //! @name Member data
    //@{
    bool pointSet_;
    geometry::Point2f point_;
    QString text_;
    QFont font_;
    //@}
};

}

#endif // __kernel_Text_h_
