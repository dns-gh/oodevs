// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Pickable_ABC_h_
#define __Pickable_ABC_h_

class QStringList;

namespace kernel
{
    class Displayer_ABC;

// =============================================================================
/** @class  Pickable_ABC
    @brief  Pickable definition
*/
// Created: BCI 2011-01-04
// =============================================================================
class Pickable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Pickable_ABC() {}
    virtual ~Pickable_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void TerrainPicked( const geometry::Point2f& /*terrainCoordinates*/, QStringList& outputInfos ) = 0;
    //@}
};

}

#endif // __Pickable_ABC_h_
