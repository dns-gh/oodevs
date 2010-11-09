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

namespace tools
{
    class SessionConfig;
};


class IndicatorRequest;

// =============================================================================
/** @class  IndicatorExportDialog
    @brief  IndicatorExportDialog
*/
// Created: SBO 2009-04-30
// =============================================================================
class IndicatorExportDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit IndicatorExportDialog( QWidget* parent, tools::SessionConfig& config );
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
    //! @name Copy/Assignment
    //@{
    IndicatorExportDialog( const IndicatorExportDialog& );            //!< Copy constructor
    IndicatorExportDialog& operator=( const IndicatorExportDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::vector< const IndicatorRequest* > T_Requests;
    //@}

private:
    //! @name Member data
    //@{
    tools::SessionConfig& config_;
    T_Requests requests_;
    QLineEdit* file_;
    QLineEdit* separator_;
    QCheckBox* header_;
    QButton* ok_;
    //@}
};

#endif // __IndicatorExportDialog_h_
