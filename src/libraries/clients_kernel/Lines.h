// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lines_h_
#define __Lines_h_

#include "Location_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  Lines
    @brief  Lines
*/
// Created: AGE 2006-08-09
// =============================================================================
class Lines : public Location_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Lines();
    virtual ~Lines();
    //@}

    //! @name Operations
    //@{
    virtual void PopPoint();
    virtual void AddPoint( const geometry::Point2f& point );
    virtual void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    virtual Location_ABC& Clone() const;
    //@}

    //! @name Accessors
    //@{
    virtual void Accept( LocationVisitor_ABC& ) const;
    virtual QString GetName() const;
    virtual bool IsValid() const;
    virtual bool IsDone() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Lines& operator=( const Lines& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    T_PointVector points_;
    //@}
};

}

#endif // __Lines_h_
