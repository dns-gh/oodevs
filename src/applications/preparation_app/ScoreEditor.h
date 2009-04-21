// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreEditor_h_
#define __ScoreEditor_h_

#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace gui
{
    class ItemFactory_ABC;
}

class Score_ABC;
class IndicatorPrimitive;
class ScoreVariablesList;

// =============================================================================
/** @class  ScoreEditor
    @brief  ScoreEditor
*/
// Created: SBO 2009-04-20
// =============================================================================
class ScoreEditor : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreEditor( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Resolver_ABC< IndicatorPrimitive, QString >& primitives );
    virtual ~ScoreEditor();
    //@}

    //! @name Operations
    //@{
    void StartEdit( Score_ABC& score );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnInsert( const QString& text );
    void OnFormulaChanged( const QString& text );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreEditor( const ScoreEditor& );            //!< Copy constructor
    ScoreEditor& operator=( const ScoreEditor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< Score_ABC > current_;
    QLineEdit* name_;
    QLineEdit* formula_;
    QButton* checkFormula_;
    ScoreVariablesList* variables_;
    //@}
};

#endif // __ScoreEditor_h_
