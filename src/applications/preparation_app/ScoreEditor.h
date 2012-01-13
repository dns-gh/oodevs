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

#include <boost/noncopyable.hpp>

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
    class Viewport_ABC;
    class GlTools_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class ParametersLayer;
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
                  , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreEditor( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const ScoresModel& model,
                          const ::StaticModel& staticModel, const kernel::GlTools_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder );
    virtual ~ScoreEditor();
    //@}

    //! @name Operations
    //@{
    void StartEdit( Score_ABC& score );
    void Draw( kernel::Viewport_ABC& viewport );
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
    Score_ABC* current_;
    QLineEdit* name_;
    Q3TextEdit* formula_;
    QLabel* checkResult_;
    QPushButton* ok_;
    QLabel* help_;
    ScoreVariablesList* variables_;
    ScoreGaugeConfiguration* gauge_;
    ScoreProfilesPage* profiles_;
    const kernel::GlTools_ABC& tools_;
    //@}
};

#endif // __ScoreEditor_h_
