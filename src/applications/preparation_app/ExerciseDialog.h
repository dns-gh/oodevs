// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseDialog_h_
#define __ExerciseDialog_h_

#include "clients_gui/ValuedComboBox.h"
#include "tools/ElementObserver_ABC.h"
#include "preparation/Exercise.h"

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class ExerciseConfig;
}

class Exercise;

// =============================================================================
/** @class  ExerciseDialog
    @brief  ExerciseDialog
*/
// Created: SBO 2010-03-09
// =============================================================================
class ExerciseDialog : public QDialog
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< Exercise >
                     , public ExerciseVisitor_ABC
                     , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseDialog( QWidget* parent, kernel::Controllers& controllers, Exercise& exercise, const tools::ExerciseConfig& config );
    virtual ~ExerciseDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const Exercise& exercise );
    virtual void VisitBriefing( const QString& lang, const QString& text );
    virtual void VisitResource( const QString& name, const QString& file );
    //@}

protected:
    //! @name From QWidget
    //@{
    virtual void showEvent( QShowEvent* showEvent );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    void OnChangeLang();
    void OnToggleDisplayMode( bool toggled );
    void OnAddResource();
    void OnDeleteResource();
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const Exercise& exercise );
    void AddResource( const QString& name, const QString& file );
    virtual QSize sizeHint() const;
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, QString > T_Briefings;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Exercise& exercise_;
    const tools::ExerciseConfig& config_;
    QLineEdit* name_;
    gui::ValuedComboBox< QString >* lang_;
    QString selectedLang_;
    Q3TextEdit* briefing_;
    T_Briefings briefings_;
    Q3ListView* resources_;
    QCheckBox* infiniteDotationsCheckBox_;
    QCheckBox* humanEvolutionCheckBox_;
    //@}
};

#endif // __ExerciseDialog_h_
