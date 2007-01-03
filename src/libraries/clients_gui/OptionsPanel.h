// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OptionsPanel_h_
#define __OptionsPanel_h_

#include <qvbox.h>
#include "clients_kernel/OptionsObserver_ABC.h"
#include "PreferencePanel_ABC.h"

class QCheckBox;

namespace kernel
{
    class Controllers;
    class Options;
}

namespace gui
{
    class Settings;

// =============================================================================
// Created: APE 2004-07-09
// =============================================================================
class OptionsPanel : public PreferencePanel_ABC
                   , public kernel::Observer_ABC
                   , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OptionsPanel( QWidget* pParent, kernel::Controllers& controllers );
    virtual ~OptionsPanel();
    //@}

    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    OptionsPanel( const OptionsPanel& );
    OptionsPanel& operator=( const OptionsPanel& );
    //@}

private slots:
    //! @name Helpers
    //@{
    void FontSizeChanged( int );
    void DrawObjectsChanged( bool );
    void DrawHoveredInfoChanged( bool );
    void DrawRCsChanged( bool );
    void DrawSubscribedRCsOnlyChanged( bool );
    void DrawMessagesChanged( bool );
    void DrawTracesChanged( bool );
    void DrawIdentificationsChanged( bool );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Options&     options_;

    QSpinBox* pFontSpinbox_;
    QCheckBox* pDrawObjectIcons_;
    QCheckBox* pDrawHoveredInfo_;
    QCheckBox* pDisplayRCOnMap_;
    QCheckBox* pDisplayMessagesOnMap_;
    QCheckBox* pDisplayTracesOnMap_;
    QCheckBox* pDisplayIdentificationLevelOnMap_ ;
    QCheckBox* pDisplayOnlySubscribedAgentsRC_ ;
    //@}
};

}

#endif // __OptionsPanel_h_
