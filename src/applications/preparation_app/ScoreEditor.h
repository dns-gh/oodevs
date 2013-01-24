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

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
    }
}

namespace indicators
{
    class GaugeTypes;
    class Primitive;
    class Primitives;
}

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
}

namespace gui
{
    class ParametersLayer;
    class Viewport_ABC;
}

class Score_ABC;
class ScoreGaugeConfiguration;
class ScoreProfilesPage;
class ScoreVariablesList;
class ScoresModel;
class StaticModel;

// =============================================================================
/** @class  ScoreEditor
    @brief  ScoreEditor
*/
// Created: SBO 2009-04-20
// =============================================================================
class ScoreEditor : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ScoreEditor( QWidget* parent, kernel::Controllers& controllers, ScoresModel& model,
                          const ::StaticModel& staticModel, const kernel::GlTools_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder );
    virtual ~ScoreEditor();
    //@}

    //! @name Operations
    //@{
    void StartEdit( Score_ABC& score );
    void Draw( gui::Viewport_ABC& viewport );
    //@}

signals:
    //! @name Signals
    //@{
    void Show();
    void Hide();
    //@}

private slots:
    //! @name Slots
    //@{
    void Commit();
    void Cancel();
    void OnInsert( const QString& text );
    void OnSelectPrimitive( const indicators::Primitive& indicator );
    void CheckFormula();
    void AllowCommit( bool base = true );
    void NameChanged();
    //@}

private:
    //! @name Helpers
    //@{
    void CommitTo( Score_ABC& score );
    virtual void closeEvent( QCloseEvent* e );
    //@}

private:
    //! @name Member data
    //@{
    ScoresModel& model_;
    Score_ABC* current_;
    QLineEdit* name_;
    QTextEdit* formula_;
    QLabel* checkResult_;
    QPushButton* ok_;
    QLabel* help_;
    ScoreVariablesList* variables_;
    ScoreGaugeConfiguration* gauge_;
    ScoreProfilesPage* profiles_;
    const kernel::GlTools_ABC& tools_;
    bool nameChanged_;
    //@}
};

#endif // __ScoreEditor_h_
