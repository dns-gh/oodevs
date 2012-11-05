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
class ADN_Sensors_ModificatorTable_ABC : public ADN_Table3
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Sensors_ModificatorTable_ABC( const QString& firstColumnName, const QString& secondColumnName, const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Sensors_ModificatorTable_ABC();
    //@}

signals:
    //! @name Signals
    //@{
    void ContentChanged( std::string name, double value );
    //@}

protected:
    //! @name Abstract Methods
    //@{
    virtual void InternalEmit() = 0;
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnCurrentChanged( const QModelIndex& index );
    //@}

protected:
    //! @name Member data
    //@{
    int lastCurrentRow_;
    //@}
};

#endif // __ADN_Sensors_ModificatorTable_h_
