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

class QGroupBox;
class QListBox;
class QSpinBox;
class QTimeEdit;

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    class CheckpointList;

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
    void Select( const QString& exercise );
    //@}

private slots:
    //! @name Operations
    //@{
    void SessionSelected( const QString& session );
    void OnCheckpointSelected( const QString& checkpoint );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CheckpointConfigPanel( const CheckpointConfigPanel& );            //!< Copy constructor
    CheckpointConfigPanel& operator=( const CheckpointConfigPanel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QTimeEdit* frequency_;
    QSpinBox* keep_;
    QGroupBox* checkpointsGroup_;
    QGroupBox* loadGroup_;
    QListBox* sessions_;
    CheckpointList* checkpoints_;
    QString exercise_;
    //@}
};

}

#endif // __CheckpointConfigPanel_h_
