// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PathContext_h_
#define __PathContext_h_

namespace svg 
{
class PathParser;

// =============================================================================
/** @class  PathContext
    @brief  PathContext
*/
// Created: AGE 2006-08-10
// =============================================================================
class PathContext 
{

public:
    //! @name Constructors/Destructor
    //@{
             PathContext();
    virtual ~PathContext();
    //@}

    //! @name Accessors
    //@{
    bool IsRelative() const;
    const geometry::Point& StartPoint() const;
    const geometry::Point& CurrentPoint() const;
    const geometry::Point& ControlPoint() const;
    //@}

    //! @name Operations
    //@{
    void SetRelative( bool relative );
    void ResetControlPoint();
    geometry::Point Translate( const geometry::Point& p );
    geometry::Point InferedControlPoint();

    void Start( const geometry::Point& p );
    void MoveTo( const geometry::Point& p );
    void SetControlPoint( const geometry::Point c );
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point startPoint_;
    geometry::Point currentPoint_;
    geometry::Point controlPoint_;
    bool relative_;
    //@}
};

}

#endif // __PathContext_h_
