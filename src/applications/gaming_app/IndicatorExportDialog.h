// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorExportDialog_h_
#define __IndicatorExportDialog_h_

class IndicatorRequest;

// =============================================================================
/** @class  IndicatorExportDialog
    @brief  IndicatorExportDialog
*/
// Created: SBO 2009-04-30
// =============================================================================
class IndicatorExportDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit IndicatorExportDialog( QWidget* parent );
    virtual ~IndicatorExportDialog();
    //@}

    //! @name Operations
    //@{
    void Add( const IndicatorRequest& request );
    void Export();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBrowse();
    void OnFileChanged( const QString& text );
    void OnAccept();
    //@}

private:
    //! @name Member data
    //@{
    std::vector< const IndicatorRequest* > requests_;
    QLineEdit* file_;
    QLineEdit* separator_;
    QCheckBox* header_;
    QPushButton* ok_;
    //@}
};

#endif // __IndicatorExportDialog_h_
