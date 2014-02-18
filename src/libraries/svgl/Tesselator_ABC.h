// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Tesselator_ABC_h_
#define __Tesselator_ABC_h_

// =============================================================================
/** @class  Tesselator_ABC
    @brief  Tesselator_ABC
*/
// Created: AGE 2006-08-12
// =============================================================================
class Tesselator_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< geometry::Point > T_Points;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Tesselator_ABC() {};
    virtual ~Tesselator_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void StartPath() = 0;
    virtual void AddPoints( const T_Points& points ) = 0;
    virtual void AddPoint( const geometry::Point& point ) = 0;
    virtual void EndPath( bool close = true ) = 0;
    //@}
};

#endif // __Tesselator_ABC_h_
