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

namespace kernel
{
    class DetectionMap;
}

namespace gui
{

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
             TerrainProfile( QWidget* parent, const kernel::DetectionMap& detection );
    virtual ~TerrainProfile();
    //@}

    //! @name Operations
    //@{
    void Update( const geometry::Point2f& from, const geometry::Point2f& to, float height, int slope );
    //@}

protected:
    bool event( QEvent* event );

private:
    //! @name Copy/Assignment
    //@{
    TerrainProfile( const TerrainProfile& );            //!< Copy constructor
    TerrainProfile& operator=( const TerrainProfile& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void UpdateVision( float height );
    void UpdateSlopes( int threshold );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& detection_;
    gui::GQ_PlotData* data_;
    gui::GQ_PlotData* vision_;
    gui::GQ_PlotData* slopes_;
    //@}
};

}

#endif // __TerrainProfile_h_
