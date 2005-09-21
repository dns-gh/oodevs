// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_ReportFilterOptions_h_
#define __MOS_ReportFilterOptions_h_

#include "MOS_Report_ABC.h"

// =============================================================================
/** @class  MOS_ReportFilterOptions
    @brief  MOS_ReportFilterOptions
*/
// Created: AGE 2005-09-21
// =============================================================================
class MOS_ReportFilterOptions : public QVBox
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_ReportFilterOptions( QWidget* pParent );
    virtual ~MOS_ReportFilterOptions();
    //@}

    //! @name Operations
    //@{
    bool ShouldDisplay( const MOS_Report_ABC& report ) const;
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
    MOS_ReportFilterOptions( const MOS_ReportFilterOptions& );            //!< Copy constructor
    MOS_ReportFilterOptions& operator=( const MOS_ReportFilterOptions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Toggle( bool, MOS_Report_ABC::E_Type type );
    //@}

private:
    //! @name Member data
    //@{
    std::set< MOS_Report_ABC::E_Type > toDisplay_;
    //@}

};

#endif // __MOS_ReportFilterOptions_h_
