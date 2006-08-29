// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __VisionMap_h_
#define __VisionMap_h_

#include "Types.h"

namespace kernel
{
    class GlTools_ABC;
    class DetectionMap;
}

// =============================================================================
/** @class  VisionMap
    @brief  VisionMap
*/
// Created: AGE 2006-04-04
// =============================================================================
class VisionMap
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit VisionMap( const kernel::DetectionMap& map );
    virtual ~VisionMap();
    //@}

    //! @name Operations
    //@{
    void Draw( const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;

    void Clear();
    bool ShouldUpdate( const std::pair< unsigned, unsigned >& cell );
    void Update( const std::pair< unsigned, unsigned >& cell, kernel::E_PerceptionResult perception );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    VisionMap( const VisionMap& );            //!< Copy constructor
    VisionMap& operator=( const VisionMap& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::pair< unsigned, unsigned > T_Cell;
    struct CellLess { 
        bool operator()( const T_Cell& lhs, const T_Cell& rhs ) const {
            return lhs.first < rhs.first || ( !( rhs.first < lhs.first) && (lhs.second < rhs.second ) );
        };
    };
    typedef std::map< T_Cell, kernel::E_PerceptionResult, CellLess > T_VisionMap;
    typedef T_VisionMap::const_iterator                            CIT_VisionMap; // $$$$ AGE 2006-04-21: mettre un allocateur
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& map_;
    geometry::Rectangle2f boundingBox_;
    T_VisionMap vision_; // $$$$ AGE 2006-04-26: use a matrix
    //@}
};

#endif // __VisionMap_h_
