// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __TerrainPreferenceWidget_h_
#define __TerrainPreferenceWidget_h_

namespace gui
{
    class TerrainPreference;
// =============================================================================
/** @class  TerrainPreferenceWidget
    @brief  TerrainPreferenceWidget
*/
// Created: LGY 2014-06-30
// =============================================================================
class TerrainPreferenceWidget : public QWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             TerrainPreferenceWidget( const QString& category, bool first, bool last, unsigned int index,
                                      const std::vector< std::shared_ptr< TerrainPreference > >& terrains,
                                      QSignalMapper* upSignalMapper, QSignalMapper* downSignalMapper );
    virtual ~TerrainPreferenceWidget();
    //@}
};

}

#endif // __TerrainPreferenceWidget_h_
