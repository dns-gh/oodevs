// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreVariablesEditor_h_
#define __ScoreVariablesEditor_h_

#include "clients_gui/ListDisplayer.h"

class IndicatorElement_ABC;
class Score_ABC;

// =============================================================================
/** @class  ScoreVariablesEditor
    @brief  ScoreVariablesEditor
*/
// Created: SBO 2009-04-20
// =============================================================================
class ScoreVariablesEditor : public QVBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreVariablesEditor( QWidget* parent, gui::ItemFactory_ABC& factory );
    virtual ~ScoreVariablesEditor();
    //@}

    //! @name Operations
    //@{
    void StartEdit( Score_ABC& score );
    void AddVariable( const IndicatorElement_ABC& variable );
    //@}

signals:
    //! @name Signals
    //@{
    void Insert( const QString& text );
    //@}

private slots:
    //! @name Operations
    //@{
    void OnAdd();
    void OnDelete();
    void OnPaste();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreVariablesEditor( const ScoreVariablesEditor& );            //!< Copy constructor
    ScoreVariablesEditor& operator=( const ScoreVariablesEditor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Display( const IndicatorElement_ABC& variable, gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    gui::ListDisplayer< ScoreVariablesEditor >* list_;
    //@}
};

#endif // __ScoreVariablesEditor_h_
