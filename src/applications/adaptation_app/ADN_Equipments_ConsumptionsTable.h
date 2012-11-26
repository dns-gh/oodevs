// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Equipments_ConsumptionsTable_h_
#define __ADN_Equipments_ConsumptionsTable_h_

#include "ADN_Table.h"
#include "ADN_Resources_Data.h"
#include <QtGui/qapplication.h>
#include <boost/noncopyable.hpp>

class ADN_ListView_Equipments;

// =============================================================================
/** @class  ADN_Equipments_ConsumptionsTable
    @brief  ADN_Equipments_ConsumptionsTable
*/
// Created: APE 2005-01-25
// =============================================================================
class ADN_Equipments_ConsumptionsTable : public ADN_Table
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipments_ConsumptionsTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Equipments_ConsumptionsTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( const QPoint& pt );
    void SetListView( ADN_ListView* listView );
    virtual void AddRow( int row, void* data );
    //@}

private:
    typedef std::pair< E_ConsumptionType, ADN_Resources_Data::CategoryInfo* > T_MenuItemParameters;
    typedef std::vector< T_MenuItemParameters >   T_ParametersVector;
    typedef T_ParametersVector::iterator         IT_ParametersVector;

private:
    //! @name Helpers
    //@{
    void CreateNewElement( T_MenuItemParameters& parameters );
    void DeleteCurrentElement();
    //@}

private:
    T_ParametersVector menuParametersList_;
    ADN_ListView* composanteListView_;
};

#endif // __ADN_Equipments_ConsumptionsTable_h_
