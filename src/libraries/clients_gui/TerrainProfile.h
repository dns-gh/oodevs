// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TerrainProfile_h_
#define __TerrainProfile_h_

#include "GQ_Plot.h"

namespace gui
{
    class TerrainProfilerLayer;
// =============================================================================
/** @class  TerrainProfile
    @brief  TerrainProfile
*/
// Created: SBO 2010-03-31
// =============================================================================
class TerrainProfile : public gui::GQ_Plot
{
public:
    //! @name Constructors/Destructor
    //@{
             TerrainProfile( QWidget* parent, TerrainProfilerLayer& layer );
    virtual ~TerrainProfile();
    //@}

    typedef struct T_PointInfo
    {
        T_PointInfo() : height_( 0 ) {}
        T_Point point_;
        QColor color_;
        float height_;
    };
    //! @name Operations
    //@{
    void Update( const std::vector< T_PointInfo >& points, bool displayHeight, int height, bool displaySlope, int slope,
                 const std::vector< geometry::Point2f >& path );
    //@}

protected:
    virtual void mouseMoveEvent( QMouseEvent* event );
    bool event( QEvent* event );

private:
    //! @name Helpers
    //@{
    void UpdateVision( int height );
    void UpdateSlopes( int threshold );
    //@}

private:
    //! @name Member data
    //@{
    TerrainProfilerLayer& layer_;
    gui::GQ_PlotData* data_;
    gui::GQ_PlotData* vision_;
    gui::GQ_PlotData* slopes_;
    gui::GQ_PlotData* selection_;
    int currentSlope_;
    int currentHeight_;
    std::vector< geometry::Point2f > path_;
    //@}
};

}

#endif // __TerrainProfile_h_
