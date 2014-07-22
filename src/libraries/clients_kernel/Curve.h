// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Curve_h_
#define __Curve_h_

#include "Location_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  Curve
    @brief  Curve
*/
// Created: LGY 2013-04-12
// =============================================================================
class Curve : public Location_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Curve();
    virtual ~Curve();
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
    virtual bool IsAt( const geometry::Point2f& point, float precision ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Curve& operator=( const Curve& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T_PointVector points_;
    //@}
};

}

#endif // __Circle_h_
