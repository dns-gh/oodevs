// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CheckpointConfigPanel_h_
#define __CheckpointConfigPanel_h_

#include "PluginConfig_ABC.h"

class Q3GroupBox;
class Q3ListBox;
class QSpinBox;
class Q3TimeEdit;
class QLabel;

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    class CheckpointList;
    class Exercise_ABC;
// =============================================================================
/** @class  CheckpointConfigPanel
    @brief  CheckpointConfigPanel
*/
// Created: SBO 2010-04-19
// =============================================================================
class CheckpointConfigPanel : public PluginConfig_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             CheckpointConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~CheckpointConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void OnLanguageChanged();
    virtual QString GetName() const;
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

signals:
    //! @name Signals
    //@{
    void CheckpointSelected( const QString& session, const QString& checkpoint );
    //@}

public slots:
    //! @name Operations
    //@{
    void Select( const frontend::Exercise_ABC& exercise );
    void ClearSelection();
    //@}

private slots:
    //! @name Operations
    //@{
    void SessionSelected( const QString& session );
    void OnCheckpointSelected( const QString& checkpoint );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig&   config_;
    Q3TimeEdit*                   frequency_;
    QSpinBox*                     keep_;
    Q3GroupBox*                   checkpointsGroup_;
    Q3GroupBox*                   loadGroup_;
    Q3ListBox*                    sessions_;
    CheckpointList*               checkpoints_;
    const frontend::Exercise_ABC* exercise_;
    QLabel*                       frequencyLabel_;
    QLabel*                       keepLabel_;
    QLabel*                       sessionLabel_;
    //@}
};
}

#endif // __CheckpointConfigPanel_h_
