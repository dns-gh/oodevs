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

namespace kernel
{
    class Controllers;
}
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
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    CreateExerciceWidget(QWidget* parent, const tools::GeneralConfig& config  );
    virtual ~CreateExerciceWidget();
    //@}
private:
    //! @name Copy/Assignment
    //@{
    CreateExerciceWidget( const CreateExerciceWidget& );            //!< Copy constructor
    CreateExerciceWidget& operator=( const CreateExerciceWidget& ); //!< Assignment operator
    //@}
private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnCancel();
    //@}
public:    
    //! @name Helpers
    //@{
    QString getFileName();
    //@}
private:
    const tools::GeneralConfig& config_;
    QLineEdit* editName_;
    QComboBox* editTerrainList_;
    QComboBox* editModelList_;
};

#endif // __CreateExerciceWidget_h_
