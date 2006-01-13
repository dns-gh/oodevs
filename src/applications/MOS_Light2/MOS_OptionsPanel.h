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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_OptionsPanel.h $
// $Author: Age $
// $Modtime: 31/03/05 16:59 $
// $Revision: 3 $
// $Workfile: MOS_OptionsPanel.h $
//
// *****************************************************************************

#ifndef __MOS_OptionsPanel_h_
#define __MOS_OptionsPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

class QCheckBox;


// =============================================================================
// Created: APE 2004-07-09
// =============================================================================
class MOS_OptionsPanel : public QWidget
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_OptionsPanel );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_OptionsPanel( QWidget* pParent );
    ~MOS_OptionsPanel();
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnCheckpoint();
    void Apply       ();
    void OnDisplayRC ();
    //@}

private:
    //! @name Member data
    //@{
    QSpinBox* pCheckpointSpinbox_ ;
    QLineEdit* pCheckpointNameEdit_;

    QSpinBox* pFontSpinbox_;

    QCheckBox* pAutoOpenCheckbox_;
    QCheckBox* pAutoSaveLoadCheckbox_;

    QCheckBox* pDrawObjectIcons_;

    QCheckBox* pDrawHoveredInfo_;

    QCheckBox* pDisplayRCOnMap_;
    QCheckBox* pDisplayMessagesOnMap_;
    QCheckBox* pDisplayTracesOnMap_;
    QCheckBox* pDisplayIdentificationLevelOnMap_ ;
    
    //@}
};

#endif // __MOS_OptionsPanel_h_
