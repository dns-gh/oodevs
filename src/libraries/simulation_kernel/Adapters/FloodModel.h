// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_FLOOD_MODEL_H
#define SWORD_FLOOD_MODEL_H

#include "EventListenerBase.h"
#include "propagation/FloodModel_ABC.h"
#include <map>

namespace sword
{
// =============================================================================
/** @class  FloodModel
    @brief  Flood model
*/
// Created: LGY 2012-06-13
// =============================================================================
class FloodModel : public propagation::FloodModel_ABC
                 , private EventListenerBase
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FloodModel( core::Facade& facade );
    virtual ~FloodModel();
    //@}

    //! @name Operations
    //@{
    virtual void GenerateFlood( const geometry::Point2d& center, T_Polygons& deepAreas, T_Polygons& lowAreas, int depth, int refDis ) const;
    virtual void GenerateFlood( const geometry::Point2f& center, T_Polygons& deepAreas, T_Polygons& lowAreas, int depth, int refDist ) const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}

private:
    //! @name Types
    //@{
    struct T_FloodEvent
    {
        T_Polygons* deepAreas_;
        T_Polygons* lowAreas_;
    };
    //@}

private:
    //! @name Helpers
    //@{
    void Fill( T_Polygons* polygons, const core::Model& areas );
    //@}

private:
    //! @name Member data
    //@{
    core::Facade& facade_;
    mutable std::size_t identifier_;
    mutable std::map< std::size_t, T_FloodEvent > floods_;
    //@}
};

}

#endif // SWORD_FLOOD_MODEL_H
