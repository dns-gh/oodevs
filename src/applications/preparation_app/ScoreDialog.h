// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreDialog_h_
#define __ScoreDialog_h_

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

namespace tools
{
    class ExerciseConfig;
}

class Score_ABC;
class ScoreList;
class ScoresModel;
class StaticModel;

// =============================================================================
/** @class  ScoreDialog
    @brief  ScoreDialog
*/
// Created: SBO 2009-04-16
// =============================================================================
class ScoreDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ScoreDialog( const QString& objectName,
                          QWidget* parent,
                          kernel::Controllers& controllers,
                          ScoresModel& model,
                          const std::shared_ptr< gui::ParametersLayer >& layer,
                          const StaticModel& staticModel,
                          const tools::ExerciseConfig& config,
                          gui::GLView_ABC& tools );
    virtual ~ScoreDialog();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Draw( gui::Viewport_ABC& viewport );
    //@}

signals:
    //! @name Signals
    //@{
    void CloseDialogs();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateTextChanged( const QString& text );
    void OnCreateButtonClicked();
    void OnDeleteScore( const Score_ABC& score );
    void OnCloseDialogs();
    //@}

private:
    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    ScoresModel& model_;
    actions::gui::InterfaceBuilder_ABC* builder_;
    ScoreList* scores_;
    gui::RichLineEdit* editor_;
    gui::RichPushButton* createButton_;
    const gui::GLView_ABC& tools_;
    //@}
};

#endif // __ScoreDialog_h_
