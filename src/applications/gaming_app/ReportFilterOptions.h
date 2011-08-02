// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportFilterOptions_h_
#define __ReportFilterOptions_h_

#include "reports/Report.h"

// =============================================================================
/** @class  ReportFilterOptions
    @brief  ReportFilterOptions
*/
// Created: AGE 2005-09-21
// =============================================================================
class ReportFilterOptions : public Q3VBox
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
    explicit ReportFilterOptions( QWidget* pParent );
    virtual ~ReportFilterOptions();
    //@}

    //! @name Operations
    //@{
    bool ShouldDisplay( const Report& report ) const;
    void AddContextMenu( Q3PopupMenu* menu ) const;
    //@}

signals:
    //! @name Signals
    //@{
    void OptionsChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnToggleRCs();
    void OnToggleTraces();
    void OnToggleEvents();
    void OnToggleMessages();
    void OnToggleWarnings();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReportFilterOptions( const ReportFilterOptions& );            //!< Copy constructor
    ReportFilterOptions& operator=( const ReportFilterOptions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Toggle( Report::E_Type type );
    void AddMenuItem( Q3PopupMenu* menu, const QString& name, Report::E_Type type, const char* slot ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::set< Report::E_Type > toDisplay_;
    //@}

};

#endif // __ReportFilterOptions_h_
