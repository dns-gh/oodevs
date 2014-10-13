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

#include "clients_gui/RichWidget.h"

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
        class RichPushButton;
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
}

namespace gui
{
    class GLView_ABC;
    class ParametersLayer;
    class Viewport_ABC;
    class RichPushButton;
    class RichLineEdit;
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
                          const ::StaticModel& staticModel, gui::GLView_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder );
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
    void CloseDialogs();
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
    void OnCloseDialogs();
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
    gui::RichLineEdit* name_;
    gui::RichWidget< QTextEdit >* formula_;
    QLabel* checkResult_;
    gui::RichPushButton* ok_;
    QLabel* help_;
    ScoreVariablesList* variables_;
    ScoreGaugeConfiguration* gauge_;
    ScoreProfilesPage* profiles_;
    const gui::GLView_ABC& tools_;
    bool nameChanged_;
    //@}
};

#endif // __ScoreEditor_h_
