// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AttributeView_h_
#define __AttributeView_h_

#include "BaseDisplayer.h"
#include "Simulation.h"
#include "ElementObserver_ABC.h"

#include <qtable.h>

class Agent;
class DataDictionary;
class Controllers;

// =============================================================================
/** @class  AttributeView
    @brief  AttributeView
*/
// Created: AGE 2006-06-22
// =============================================================================
class AttributeView : public QTable, public BaseDisplayer, public Observer_ABC
                    , public ElementObserver_ABC< Simulation::sEndTick >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AttributeView( QWidget* parent, Controllers& controllers, const Agent& agent );
    virtual ~AttributeView();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AttributeView( const AttributeView& );            //!< Copy constructor
    AttributeView& operator=( const AttributeView& ); //!< Assignement operator
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( int, int );
    //@}

private:
    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    virtual void NotifyUpdated( const Simulation::sEndTick& );
    virtual void keyPressEvent( QKeyEvent* );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const DataDictionary& dictionary_;
    int currentRow_;
    QString message_;
    //@}
};

#endif // __AttributeView_h_
