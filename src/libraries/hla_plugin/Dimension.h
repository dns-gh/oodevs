// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dimension_h_
#define __Dimension_h_

// =============================================================================
/** @class  Dimension
    @brief  Dimension
*/
// Created: AGE 2008-02-21
// =============================================================================
class Dimension
{

public:
    //! @name Constructors/Destructor
    //@{
             Dimension();
    virtual ~Dimension();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Dimension( const Dimension& );            //!< Copy constructor
    Dimension& operator=( const Dimension& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    float xAxisLength_, yAxisLength_, zAxisLength_;
    //@}
};

#endif // __Dimension_h_
