// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_Resources_Tables_h_
#define __ADN_Equipments_Resources_Tables_h_

class ADN_Connector_ABC;
class ADN_ListView;
class ADN_Table;

// =============================================================================
/** @class  ADN_Equipments_Resources_Tables
    @brief  ADN_Equipments_Resources_Tables
*/
// Created: ABR 2012-11-30
// =============================================================================
class ADN_Equipments_Resources_Tables : public QWidget
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipments_Resources_Tables( const QString& objectName, ADN_Connector_ABC*& dotationConnector, ADN_Connector_ABC*& allocationConnector, ADN_Connector_ABC*& consumptionConnector, QWidget* parent = 0 );
    virtual ~ADN_Equipments_Resources_Tables();
    //@}

    //! @name Operations
    //@{
    const ADN_Table& GetAllocationTable() const;
    const ADN_Table& GetConsumptionTable() const;
    void ResetScrollBars();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Table* categoryList_;
    ADN_Table* allocationTable_;
    ADN_Table* consumptionTable_;
    //@}
};

#endif // __ADN_Equipments_Resources_Tables_h_
