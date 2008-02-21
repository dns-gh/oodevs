// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Coordinates_h_
#define __Coordinates_h_

// =============================================================================
/** @class  WorldLocation
    @brief  WorldLocation
*/
// Created: AGE 2008-02-21
// =============================================================================
class WorldLocation
{

public:
    //! @name Constructors/Destructor
    //@{
             WorldLocation();
    virtual ~WorldLocation();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

// =============================================================================
/** @class  Orientation
    @brief  Orientation
*/
// Created: AGE 2008-02-21
// =============================================================================
class Orientation
{

public:
    //! @name Constructors/Destructor
    //@{
             Orientation();
    virtual ~Orientation();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Member data
    //@{
    float psi_, tetha_, phi_;
    //@}
};

// =============================================================================
/** @class  VelocityVector
    @brief  VelocityVector
*/
// Created: AGE 2008-02-21
// =============================================================================
class VelocityVector
{

public:
    //! @name Constructors/Destructor
    //@{
             VelocityVector();
    virtual ~VelocityVector();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Member data
    //@{
    float xv_, yv_, zv_;
    //@}
};

#endif // __Coordinates_h_
