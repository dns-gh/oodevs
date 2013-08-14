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
#include "clients_kernel/OptionsObserver_ABC.h"
#include "SoundManager.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class RichSlider;
    class RichLineEdit;

class SoundPlayer
{
public:
    virtual ~SoundPlayer() {}
    virtual void SetVolume( const std::string& channel, double value ) = 0;
    virtual void ChangeSoundsDirectory( const tools::Path& path ) = 0;
};

// =============================================================================
/** @class  SoundPanel
    @brief  SoundPanel
*/
// Created: NPT 2013-07-05
// =============================================================================
class SoundPanel : public PreferencePanel_ABC
                 , public tools::Observer_ABC
                 , public kernel::OptionsObserver_ABC
{
Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    SoundPanel( QWidget* parent, kernel::Controllers& controllers, SoundPlayer& soundPlayer );
    virtual ~SoundPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Reset();
    virtual void Commit();
    std::map< std::string, RichSlider*>& GetSoundSliders();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
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
    std::map< std::string, int > soundValues_;
    tools::Path soundDirectory_;
    kernel::Controllers& controllers_;
    SoundPlayer& soundPlayer_;
    //@}
};

}

#endif // __SoundPanel_h_
