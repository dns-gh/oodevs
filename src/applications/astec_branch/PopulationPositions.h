// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationPositions_h_
#define __PopulationPositions_h_

#include "Positions.h"

// =============================================================================
/** @class  PopulationPositions
    @brief  PopulationPositions
*/
// Created: AGE 2006-03-23
// =============================================================================
class PopulationPositions : public Positions
{
    
public:
    //! @name Constructors/Destructor
    //@{
             PopulationPositions();
    virtual ~PopulationPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationPositions( const PopulationPositions& );            //!< Copy constructor
    PopulationPositions& operator=( const PopulationPositions& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __PopulationPositions_h_
