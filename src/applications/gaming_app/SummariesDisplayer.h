// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SummariesDisplayer_h_
#define __SummariesDisplayer_h_

#include "clients_gui/BaseDisplayer.h"

// =============================================================================
/** @class  SummariesDisplayer
    @brief  Summaries displayer
*/
// Created: SBO 2007-03-01
// =============================================================================
class SummariesDisplayer : public gui::BaseDisplayer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SummariesDisplayer( QWidget* parent );
    virtual ~SummariesDisplayer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual kernel::Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    virtual void Hide();
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, QLabel* > T_Labels;
    typedef T_Labels::const_iterator   CIT_Labels;
    //@}

private:
    //! @name Member data
    //@{
    QWidget* parent_;
    QString currentItem_;
    QString message_;
    T_Labels names_;
    T_Labels values_;
    //@}
};

#endif // __SummariesDisplayer_h_
