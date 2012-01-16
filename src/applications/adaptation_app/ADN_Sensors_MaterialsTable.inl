// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_MaterialsTable constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
template< typename ConnectorType >
ADN_Sensors_MaterialsTable< ConnectorType >::ADN_Sensors_MaterialsTable( QWidget* pParent, const char* szName, const QString& firstColumnName, const QString& secondColumnName )
    : ADN_Sensors_MaterialsTable_ABC( pParent, szName, firstColumnName, secondColumnName )
{
    pConnector_ = new ConnectorType( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_MaterialsTable destructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
template< typename ConnectorType >
ADN_Sensors_MaterialsTable< ConnectorType >::~ADN_Sensors_MaterialsTable()
{
    delete pConnector_;
}
