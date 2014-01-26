// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CheckpointList_h_
#define __CheckpointList_h_

#include "clients_gui/WidgetLanguageObserver_ABC.h"
#include <QtGui/QLabel>

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

// =============================================================================
/** @class  CheckpointList
    @brief  CheckpointList
*/
// Created: SBO 2010-04-21
// =============================================================================
class CheckpointList : public gui::WidgetLanguageObserver_ABC< QWidget >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             CheckpointList( const tools::GeneralConfig& config );
    virtual ~CheckpointList();
    //@}

    //! @name Operations
    //@{
    void Update( const tools::Path& exercice, const tools::Path& session );
    void ClearSelection();
    virtual void OnLanguageChanged();
    //@}

signals:
    //! @name Signals
    //@{
    void Select( const tools::Path& checkpoint );
    //@}

public slots:
    //! @name Slots
    //@{
    void Toggle( bool enabled );
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectCheckpoint( int index );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QListWidget* list_;
    QLabel* label_;
    tools::Path exercise_;
    tools::Path session_;
    tools::Path::T_Paths checkpoints_;
    bool enabled_;
    //@}
};

}

#endif // __CheckpointList_h_
