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

namespace kernel
{
    class Positions;

// =============================================================================
/** @class  Path
    @brief  Path
*/
// Created: AGE 2006-08-09
// =============================================================================
class Path : public Location_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Path( const Positions& position );
    virtual ~Path();
    //@}

    //! @name Construction
    //@{
    virtual void PopPoint();
    virtual void AddPoint( const geometry::Point2f& point );
    virtual bool IsValid() const;
    virtual bool IsDone() const;
    //@}

    //! @name Operations
    //@{
    virtual void Accept( LocationVisitor_ABC& ) const;
    virtual void Draw( const GlTools_ABC& tools ) const;
    virtual QString GetName() const;
    virtual Location_ABC& Clone() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Path& operator=( const Path& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const Positions& position_;
    T_PointVector points_;
    //@}
};

}

#endif // __Path_h_
