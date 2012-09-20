 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BurnSurfaceAttribute_h_
#define __BurnSurfaceAttribute_h_

#include "ObjectAttribute_ABC.h"
#include <geometry/types.h>

namespace sword
{
    enum EnumBurningCellPhase;
}

namespace dispatcher
{
// =============================================================================
/** @class  BurnSurfaceAttribute
    @brief  BurnSurfaceAttribute
*/
// Created: BCI 2010-12-22
// =============================================================================
class BurnSurfaceAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit BurnSurfaceAttribute( const sword::ObjectAttributes& msg );
    virtual ~BurnSurfaceAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& msg );
    virtual void Send( sword::ObjectAttributes& msg ) const;
    //@}

private:
    //! @name Operations
    //@{
    void DoUpdate( const sword::ObjectAttributes& msg );
    //@}

private:
    //! @name Types
    //@{
    typedef geometry::Point2< int > BurningCellOrigin;

    // $$$$ RC LDC This should be replaced with different classes, one per phase
    // in order not to clutter memory with several uninitialised and unused integers.
    struct BurningCell
    {
        BurningCell()
            : bSendFullState_( false )
        {
        }
        sword::EnumBurningCellPhase phase_;
        int ignitionEnergy_;
        int ignitionThreshold_;
        int combustionEnergy_;
        int currentHeat_;
        int maxCombustionEnergy_;
        bool bSendFullState_;
    };
    //@}

private:
    //! @name Member data
    //@{
    typedef std::map< BurningCellOrigin, BurningCell > BurningCellsByCoordinatesMap;
    BurningCellsByCoordinatesMap burningCellsByCoordinates_;
    int cellSize_;
    //@}
};

}

#endif // __BurnSurfaceAttribute_h_
