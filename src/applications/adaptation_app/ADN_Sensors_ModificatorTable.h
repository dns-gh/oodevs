// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Sensors_ModificatorTable_h_
#define __ADN_Sensors_ModificatorTable_h_

#include "ADN_CommonGfx.h"

// =============================================================================
/** @class  ADN_Sensors_ModificatorTable_ABC
    @brief  ADN_Sensors_ModificatorTable_ABC
*/
// Created: ABR 2012-01-16
// =============================================================================
class ADN_Sensors_ModificatorTable_ABC : public ADN_Table2
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Sensors_ModificatorTable_ABC( QWidget* pParent, const char* szName, const QString& firstColumnName, const QString& secondColumnName );
    virtual ~ADN_Sensors_ModificatorTable_ABC();
    //@}

signals:
    //! @name Signals
    //@{
    void ContentChanged( std::string name, double value );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual QWidget* createEditor( int row, int col, bool initFromCell ) const;
    virtual void paintCell ( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg );
    //@}

    //! @name Abstract Methods
    //@{
    virtual void InternalEmit() = 0;
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnCurrentChanged();
    //@}

protected:
    //! @name Member data
    //@{
    int lastCurrentRow_;
    //@}
};

// =============================================================================
/** @class  ADN_Sensors_ModificatorTable
    @brief  ADN_Sensors_ModificatorTable
*/
// Created: ABR 2012-01-16
// =============================================================================
template< typename ConnectorType >
class ADN_Sensors_ModificatorTable : public ADN_Sensors_ModificatorTable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Sensors_ModificatorTable( QWidget* pParent, const char* szName, const QString& firstColumnName, const QString& secondColumnName );
    virtual ~ADN_Sensors_ModificatorTable();
    //@}
};

#include "ADN_Sensors_ModificatorTable.inl"

#endif // __ADN_Sensors_ModificatorTable_h_
