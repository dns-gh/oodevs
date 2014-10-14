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
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TerrainPreferenceWidget( const QString& category,
                                      const std::vector< TerrainPreference* >& terrains,
                                      QSignalMapper* upSignalMapper,
                                      QSignalMapper* downSignalMapper );
    virtual ~TerrainPreferenceWidget();
    //@}

    //! @name Operations
    //@{
    void UpdateUpDownButton( bool enableUp, bool enableDown );
    //@}

private:
    //! @name Member data
    //@{
    QPushButton* up_;
    QPushButton* down_;
    //@}
};

} //! namespace gui

#endif // __TerrainPreferenceWidget_h_
