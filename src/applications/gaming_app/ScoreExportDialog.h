// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreExportDialog_h_
#define __ScoreExportDialog_h_

class IndicatorRequest;

// =============================================================================
/** @class  ScoreExportDialog
    @brief  ScoreExportDialog
*/
// Created: SBO 2009-04-30
// =============================================================================
class ScoreExportDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit ScoreExportDialog( QWidget* parent );
    virtual ~ScoreExportDialog();
    //@}

    //! @name Operations
    //@{
    void Export( const IndicatorRequest& request );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBrowse();
    void OnFileChanged( const QString& text );
    void OnAccept();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreExportDialog( const ScoreExportDialog& );            //!< Copy constructor
    ScoreExportDialog& operator=( const ScoreExportDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const IndicatorRequest* current_;
    QLineEdit* file_;
    QLineEdit* separator_;
    QCheckBox* header_;
    QButton* ok_;
    //@}
};

#endif // __ScoreExportDialog_h_
