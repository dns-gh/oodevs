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
class ADN_MissionGenObjectTypes_Table : public ADN_Table2
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_MissionGenObjectTypes_Table( QCheckBox* all, const QString& objectName, QWidget* pParent = 0 );
    virtual ~ADN_MissionGenObjectTypes_Table();
    //@}

public slots:
    //! @name slots
    //@{
    void OnTypeChanged( E_MissionParameterType type );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void FinalizeConnection();
    virtual void doValueChanged( int row, int col );
    //@}


    //! @name Operations
    //@{
    //virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
    //virtual void AddRow( int row, void* data );
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
