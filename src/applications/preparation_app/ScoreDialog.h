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

#include <boost/noncopyable.hpp>

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
    class ItemFactory_ABC;
    class ParametersLayer;
}

class Score_ABC;
class ScoresModel;
class StaticModel;

// =============================================================================
/** @class  ScoreDialog
    @brief  ScoreDialog
*/
// Created: SBO 2009-04-16
// =============================================================================
class ScoreDialog : public QDialog
                  , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ScoresModel& model, gui::ParametersLayer& layer, const StaticModel& staticModel );
    virtual ~ScoreDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateTextChanged( const QString& text );
    void OnCreateButtonClicked();
    void OnDeleteScore( const Score_ABC& score );
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
    QLineEdit* editor_;
    QButton* createButton_;
    //@}
};

#endif // __ScoreDialog_h_
