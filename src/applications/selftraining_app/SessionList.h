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
#include "clients_gui/LanguageChangeObserver_ABC.h"

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
class SessionList : public gui::LanguageChangeObserver_ABC< Q3VBox >
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
    void Update( const QString& exercice );
    //@}

signals:
    //! @name Signals
    //@{
    void Select( const QString& session );
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
    void ReadComments( const QString& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC& fileLoader_;
    Q3ListBox* list_;
    Q3TextEdit* comments_;
    QString exercise_;
    QLabel* sessionLabel_;
    //@}
};

#endif // __SessionList_h_
