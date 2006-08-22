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
    virtual std::string GetName() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Lines( const Lines& );            //!< Copy constructor
    Lines& operator=( const Lines& ); //!< Assignement operator
    //@}

protected:
    //! @name Member data
    //@{
    T_PointVector points_;
    //@}
};

}

#endif // __Lines_h_
