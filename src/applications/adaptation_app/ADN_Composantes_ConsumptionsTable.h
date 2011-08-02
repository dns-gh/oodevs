// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Composantes_ConsumptionsTable_h_
#define __ADN_Composantes_ConsumptionsTable_h_

#include "ADN_Table.h"
#include "ADN_Equipement_Data.h"
#include <QtGui/qapplication.h>
#include <boost/noncopyable.hpp>

class ADN_ListView_Composantes;

// =============================================================================
/** @class  ADN_Composantes_ConsumptionsTable
    @brief  ADN_Composantes_ConsumptionsTable
    @par    Using example
    @code
    ADN_Composantes_ConsumptionsTable;
    @endcode
*/
// Created: APE 2005-01-25
// =============================================================================
class ADN_Composantes_ConsumptionsTable : public ADN_Table2
                                        , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Composantes_ConsumptionsTable( ADN_ListView_Composantes& composanteListView, QWidget* pParent = 0 );
    virtual ~ADN_Composantes_ConsumptionsTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    //@}

private:
    typedef std::pair< E_ConsumptionType, ADN_Equipement_Data::CategoryInfo* > T_MenuItemParameters;
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
    ADN_ListView_Composantes& composanteListView_;
};


#endif // __ADN_Composantes_ConsumptionsTable_h_
