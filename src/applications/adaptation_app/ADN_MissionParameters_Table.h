// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_MissionParameters_Table_h_
#define __ADN_MissionParameters_Table_h_

#include "ADN_CommonGfx.h"

// =============================================================================
/** @class  ADN_MissionParameters_Table
    @brief  ADN_MissionParameters_Table
*/
// Created: SBO 2006-12-04
// =============================================================================
class ADN_MissionParameters_Table : public ADN_Table2
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_MissionParameters_Table( QWidget* parent = 0 );
    virtual ~ADN_MissionParameters_Table();
    //@}

    //! @name Operations
    //@{
    void SetItemConnectors( const T_ConnectorVector& itemConnectors );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_MissionParameters_Table( const ADN_MissionParameters_Table& );            //!< Copy constructor
    ADN_MissionParameters_Table& operator=( const ADN_MissionParameters_Table& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AddNewElement();
    void RemoveCurrentElement();
    virtual void OnContextMenu( int row, int col, const QPoint& pt );
    //@}

private:
    //! @name Member data
    //@{
    T_ConnectorVector itemConnectors_;
    //@}
};

#endif // __ADN_MissionParameters_Table_h_
