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

#include "gaming/Report_ABC.h"

// =============================================================================
/** @class  ReportFilterOptions
    @brief  ReportFilterOptions
*/
// Created: AGE 2005-09-21
// =============================================================================
class ReportFilterOptions : public QVBox
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
    bool ShouldDisplay( const Report_ABC& report ) const;
    //@}

signals:
    //! @name Signals
    //@{
    void OptionsChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnToggleRCs( bool );
    void OnToggleTraces( bool );
    void OnToggleEvents( bool );
    void OnToggleMessages( bool );
    void OnToggleWarnings( bool );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ReportFilterOptions( const ReportFilterOptions& );            //!< Copy constructor
    ReportFilterOptions& operator=( const ReportFilterOptions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Toggle( bool, Report_ABC::E_Type type );
    //@}

private:
    //! @name Member data
    //@{
    std::set< Report_ABC::E_Type > toDisplay_;
    //@}

};

#endif // __ReportFilterOptions_h_
