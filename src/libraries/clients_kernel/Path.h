// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Path_h_
#define __Path_h_

#include "Location_ABC.h"
#include <boost/optional.hpp>

namespace kernel
{
    class Positions;

// =============================================================================
/** @class  Path
    @brief  Path
    // $$$$ SBO 2008-05-30: origin point not included into points_...
    // $$$$ SBO 2008-05-30: watch for consequences in drawing, IsAt, Translate...
*/
// Created: AGE 2006-08-09
// =============================================================================
class Path : public Location_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Path( const Positions* position );
    virtual ~Path();
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
    void SetEntityPosition( const Positions* position );
    void FixOrigin( bool dynamicVisitor );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Path& operator=( const Path& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Positions* position_;
    boost::optional< geometry::Point2f > origin_;
    T_PointVector points_;
    //@}
};

}

#endif // __Path_h_
