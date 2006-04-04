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

#include "Perception_Def.h"

class GlTools_ABC;

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
             VisionMap();
    virtual ~VisionMap();
    //@}

    //! @name Operations
    //@{
    void Draw( const GlTools_ABC& tools ) const;
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
    typedef std::map< T_Cell, E_PerceptionResult, CellLess > T_VisionMap;
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __VisionMap_h_
