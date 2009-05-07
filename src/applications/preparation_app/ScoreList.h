// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreList_h_
#define __ScoreList_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_gui/ListDisplayer.h"

namespace indicators
{
    class GaugeTypes;
    class Primitives;
}

namespace kernel
{
    class Controllers;
}

class Score_ABC;
class ScoreEditor;

// =============================================================================
/** @class  ScoreList
    @brief  ScoreList
*/
// Created: SBO 2009-04-16
// =============================================================================
class ScoreList : public QVBox
                , public kernel::Observer_ABC
                , public kernel::ElementObserver_ABC< Score_ABC > 
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const indicators::Primitives& indicators, const indicators::GaugeTypes& gauges );
    virtual ~ScoreList();
    //@}

signals:
    //! @name Signals
    //@{
    void ScoreDeleted( const Score_ABC& score );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEdit();
    void OnDelete();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreList( const ScoreList& );            //!< Copy constructor
    ScoreList& operator=( const ScoreList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Score_ABC& element );
    virtual void NotifyUpdated( const Score_ABC& element );
    virtual void NotifyDeleted( const Score_ABC& element );
    void Display( const Score_ABC& score, gui::ValuedListItem* item );
    Score_ABC* FindSelected() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    gui::ListDisplayer< ScoreList >* scores_;
    ScoreEditor* editor_;
    QButton* editButton_;
    QButton* deleteButton_;
    //@}
};

#endif // __ScoreList_h_
