// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreVariablesList_h_
#define __ScoreVariablesList_h_

#include "clients_gui/ListDisplayer.h"

namespace indicators
{
    class Element_ABC;
    class Variables;
}

class Score_ABC;
class ScoreVariableCreationWizard;

// =============================================================================
/** @class  ScoreVariablesList
    @brief  ScoreVariablesList
*/
// Created: SBO 2009-04-20
// =============================================================================
class ScoreVariablesList : public QVBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreVariablesList( QWidget* parent, gui::ItemFactory_ABC& factory );
    virtual ~ScoreVariablesList();
    //@}

    //! @name Operations
    //@{
    void StartEdit( const Score_ABC& score );
    indicators::Variables GetValue() const;
    //@}

signals:
    //! @name Signals
    //@{
    void Insert( const QString& text );
    void Updated();
    //@}

public slots:
    //! @name Slots
    //@{
    void AddVariable( const indicators::Element_ABC& variable );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAdd();
    void OnDelete();
    void OnPaste();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreVariablesList( const ScoreVariablesList& );            //!< Copy constructor
    ScoreVariablesList& operator=( const ScoreVariablesList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Display( const indicators::Element_ABC& variable, gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    ScoreVariableCreationWizard* wizard_;
    gui::ListDisplayer< ScoreVariablesList >* list_;
    //@}
};

#endif // __ScoreVariablesList_h_
