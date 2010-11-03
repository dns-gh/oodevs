// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CreateExerciceWidget_h_
#define __CreateExerciceWidget_h_

#include <boost/noncopyable.hpp>

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  CreateExerciceWidget
    @brief  CreateExerciceWidget
*/
// Created: FDS 2010-11-02
// =============================================================================
class CreateExerciceWidget : public QDialog
                           , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             CreateExerciceWidget( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~CreateExerciceWidget();
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
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QLineEdit* editName_;
    QComboBox* editTerrainList_;
    QComboBox* editModelList_;
    //@}
};

#endif // __CreateExerciceWidget_h_
