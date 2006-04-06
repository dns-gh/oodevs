// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DetectionMapIterator_h_
#define __DetectionMapIterator_h_

class DetectionMap;

// =============================================================================
/** @class  DetectionMapIterator
    @brief  DetectionMapIterator
*/
// Created: AGE 2006-04-06
// =============================================================================
class DetectionMapIterator
{

public:
    //! @name Constructors/Destructor
    //@{
             DetectionMapIterator( const DetectionMap& map, const geometry::Point2f& from, const geometry::Point2f& to );
    virtual ~DetectionMapIterator();
    //@}

    //! @name Operations
    //@{
    void Increment();

    bool IsDone() const;
    short Altitude() const;
    short Elevation() const;
    bool IsInTown() const;
    bool IsInForest() const;

    geometry::Point2f CurrentPoint() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DetectionMapIterator( const DetectionMapIterator& );            //!< Copy constructor
    DetectionMapIterator& operator=( const DetectionMapIterator& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const DetectionMap& map_;
    float cellSize_;

    std::pair< unsigned, unsigned > endCell_;
    float xEnd_, yEnd_;

    std::pair< unsigned, unsigned > currentCell_;
    float xCurrent_, yCurrent_;

    float xOffset_, yOffset_;
    float xIncrease_, yIncrease_;
    //@}
};

#endif // __DetectionMapIterator_h_
