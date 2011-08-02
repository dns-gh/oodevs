// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseCreationDialog_h_
#define __ExerciseCreationDialog_h_

#include <boost/noncopyable.hpp>

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  ExerciseCreationDialog
    @brief  ExerciseCreationDialog
*/
// Created: FDS 2010-11-02
// =============================================================================
class ExerciseCreationDialog : public QDialog
                             , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseCreationDialog( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~ExerciseCreationDialog();
    //@}

    //! @name Accessors
    //@{
    QString GetFileName() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnCancel();
    void OnFileChanged();
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig&  config_;
    QLineEdit*                   exerciseName_;
    QComboBox*                   editTerrainList_;
    QComboBox*                   editModelList_;
    QPushButton*                     ok_;
    //@}
};

#endif // __ExerciseCreationDialog_h_
