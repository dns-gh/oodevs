// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-07-09 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/OptionsPanel.h $
// $Author: Age $
// $Modtime: 31/03/05 16:59 $
// $Revision: 3 $
// $Workfile: OptionsPanel.h $
//
// *****************************************************************************

#ifndef __OptionsPanel_h_
#define __OptionsPanel_h_

#include "OptionsObserver_ABC.h"

class QCheckBox;
class Controllers;
class Options;
class Settings;

// =============================================================================
// Created: APE 2004-07-09
// =============================================================================
class OptionsPanel : public QWidget
                   , private Observer_ABC
                   , public OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OptionsPanel( QWidget* pParent, Controllers& controllers );
    virtual ~OptionsPanel();
    //@}

    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
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
    Controllers& controllers_;
    Options&     options_;

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

#endif // __OptionsPanel_h_
