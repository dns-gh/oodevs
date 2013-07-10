// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __SoundPanel_h_
#define __SoundPanel_h_

#include "PreferencePanel_ABC.h"
#include "RichLineEdit.h"



namespace gui
{
    class RichSlider;
// =============================================================================
/** @class  SoundPanel
    @brief  SoundPanel
*/
// Created: NPT 2013-07-05
// =============================================================================
class SoundPanel : public PreferencePanel_ABC
{
Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SoundPanel( QWidget* parent );
    virtual ~SoundPanel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Helpers
    //@{
    //@}
signals:
    void soundSliderUpdated( const std::string name, int val );

public slots:
    void OnSliderChanged( int value );
    void OnChangeVolume( const std::string& name, int value );
    void OnChooseSoundsDirectory();
private:
    //! @name Member data
    //@{
    std::map< std::string, RichSlider*> soundSliders_;
    gui::RichLineEdit*  soundDirectoryEditor_;
    //@}
};

}

#endif // __SoundPanel_h_
