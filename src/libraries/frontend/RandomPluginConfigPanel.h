    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __RandomPluginConfigPanel_h_
#define __RandomPluginConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QComboBox;
class QCheckBox;
class QLineEdit;
class QSpinBox;
class QLabel;
class QPushButton;

namespace frontend
{
// =============================================================================
/** @class  RandomPluginConfigPanel
    @brief  RandomPluginConfigPanel
*/
// Created: JSR 2010-07-12
// =============================================================================
class RandomPluginConfigPanel : public PluginConfig_ABC
{
    Q_OBJECT

public:
    enum ERandomContexts
    {
        eFire,
        eWounds,
        ePerception,
        eBreakdowns,
        eContextsNbr
    };

public:
    //! @name Constructors/Destructor
    //@{
             RandomPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~RandomPluginConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void OnLanguageChanged();
    virtual QString GetName() const;
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadRandomValues();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnContextChanged( int index );
    void OnDistributionChanged( int index );
    void OnDeviationChanged( const QString& );
    void OnMeanChanged( const QString& );
    void OnSeedToggled();
    void OnDefaultClicked();
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QComboBox*                  contextList_;
    QComboBox*                  distributionList_;
    QLineEdit*                  deviation_;
    QLineEdit*                  mean_;
    QCheckBox*                  hasSeed_;
    QSpinBox*                   seed_;
    bool                        bDistributions_[ eContextsNbr ];
    double                      rDeviations_[ eContextsNbr ];
    double                      rMeans_[ eContextsNbr ];
    QGroupBox*                  mainBox_;
    QLabel*                     contextLabel_;
    QLabel*                     distributionLabel_;
    QLabel*                     standardDeviationLabel_;
    QLabel*                     meanLabel_;
    QPushButton*                defaultButton_;
    //@}
};
}

#endif // __RandomPluginConfigPanel_h_
