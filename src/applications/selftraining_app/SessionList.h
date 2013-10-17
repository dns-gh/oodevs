// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SessionList_h_
#define __SessionList_h_

#include <boost/noncopyable.hpp>
#include "clients_gui/WidgetLanguageObserver_ABC.h"

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

// =============================================================================
/** @class  SessionList
    @brief  SessionList
*/
// Created: SBO 2009-12-13
// =============================================================================
class SessionList : public gui::WidgetLanguageObserver_ABC< QWidget >
                  , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SessionList( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader );
    virtual ~SessionList();
    //@}

    //! @name Operations
    //@{
    void Update( const tools::Path& exercice );
    //@}

signals:
    //! @name Signals
    //@{
    void Select( const tools::Path& session );
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectSession( int index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    void ReadComments( const tools::Path& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC& fileLoader_;
    QListWidget* list_;
    QTextEdit* comments_;
    tools::Path exercise_;
    QLabel* sessionLabel_;
    //@}
};

#endif // __SessionList_h_
