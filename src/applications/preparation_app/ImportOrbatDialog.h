// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ImportOrbatDialog_h_
#define __ImportOrbatDialog_h_

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

class Model;
class QLineEdit;
class QListView;

// =============================================================================
/** @class  ImportOrbatDialog
    @brief  ImportOrbatDialog
*/
// Created: SBO 2008-04-07
// =============================================================================
class ImportOrbatDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ImportOrbatDialog( QWidget* parent, const tools::ExerciseConfig& config, Model& model );
    virtual ~ImportOrbatDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    void OnBrowse();
    void ToggleObjects( bool toggle );
    void TogglePopulations( bool toggle );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ImportOrbatDialog( const ImportOrbatDialog& );            //!< Copy constructor
    ImportOrbatDialog& operator=( const ImportOrbatDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void LoadPreview();
    void ReadTeam( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    Model& model_;
    QLineEdit* filename_;
    QListView* preview_;
    QPushButton* okButton_;
    bool importObjects_;
    bool importPopulations_;
    //@}
};

#endif // __ImportOrbatDialog_h_
