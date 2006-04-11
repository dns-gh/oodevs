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

class QCheckBox;

// =============================================================================
// Created: APE 2004-07-09
// =============================================================================
class OptionsPanel : public QWidget
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( OptionsPanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit OptionsPanel( QWidget* pParent );
    virtual ~OptionsPanel();
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
    QCheckBox* pDisplayOnlySubscribedAgentsRC_;
    QCheckBox* pAskForTacticalLineSavingOnExit_;
    
    //@}
};

#endif // __OptionsPanel_h_
