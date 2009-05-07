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

namespace indicators
{
    class GaugeTypes;
    class Primitive;
    class Primitives;
}

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

class Score_ABC;
class ScoreGaugeConfiguration;
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
             ScoreEditor( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const indicators::Primitives& indicators, const indicators::GaugeTypes& gauges );
    virtual ~ScoreEditor();
    //@}

    //! @name Operations
    //@{
    void StartEdit( Score_ABC& score );
    //@}

private slots:
    //! @name Slots
    //@{
    void Commit();
    void OnInsert( const QString& text );
    void OnSelectPrimitive( const indicators::Primitive& indicator );
    void OnFormulaChanged( const QString& text );
    void CheckFormula();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreEditor( const ScoreEditor& );            //!< Copy constructor
    ScoreEditor& operator=( const ScoreEditor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CommitTo( Score_ABC& score );
    //@}

private:
    //! @name Member data
    //@{
    const indicators::Primitives& indicators_;
    Score_ABC* current_;
    QLineEdit* name_;
    QLineEdit* formula_;
    QLabel* checkResult_;
    QButton* ok_;
    QLabel* help_;
    ScoreVariablesList* variables_;
    ScoreGaugeConfiguration* gauge_;
    //@}
};

#endif // __ScoreEditor_h_
