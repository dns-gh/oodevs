// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_MissionGenObjectTypes_Table_h_
#define __ADN_MissionGenObjectTypes_Table_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_MissionGenObjectTypes_Table
    @brief  ADN_MissionGenObjectTypes_Table
*/
// Created: LGY 2012-04-18
// =============================================================================
class ADN_MissionGenObjectTypes_Table : public ADN_Table
                                      , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_MissionGenObjectTypes_Table( QWidget* pParent, ADN_Connector_ABC*& pGuiConnector, QCheckBox* all );
    virtual ~ADN_MissionGenObjectTypes_Table();
    //@}

public slots:
    //! @name slots
    //@{
    void OnTypeChanged( E_MissionParameterType type );
    //@}

private:
    //! @name Operations
    //@{
    virtual void doValueChanged( int row, int col );
    //@}

private:
    //! @name Helpers
    //@{
    bool IsChecked() const;
    //@}

private slots:
    //! @name slots
    //@{
    void OnStateChanged( int state );
    //@}

private:
    //! @name Data Member
    //@{
    QCheckBox* all_;
    //@}
};

#endif // __ADN_MissionGenObjectTypes_Table_h_
