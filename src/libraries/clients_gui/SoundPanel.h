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

namespace kernel
{
    class OptionsController;
}

namespace gui
{

class SoundPlayer
{
public:
    virtual ~SoundPlayer() {}
    virtual void SetVolume( const std::string& channel, double value ) = 0;
    virtual void ChangeSoundsDirectory( const tools::Path& path ) = 0;
    virtual void SetSoundState( bool enabled ) = 0;
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
             SoundPanel( QWidget* parent,
                         kernel::OptionsController& options,
                         SoundPlayer& soundPlayer );
    virtual ~SoundPanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
     void SetSoundDirectory( const tools::Path& path );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnChooseSoundsDirectory();
    void OnSoundsDirectoryChanged( const QString& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& options_;
    SoundPlayer& soundPlayer_;
    QLineEdit* soundDirectoryEditor_;
    //@}
};

}

#endif // __SoundPanel_h_
