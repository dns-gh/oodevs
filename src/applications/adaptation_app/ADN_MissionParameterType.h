// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_MissionParameterType_h_
#define __ADN_MissionParameterType_h_

#include "ADN_TableItem_ComboBox.h"

// =============================================================================
/** @class  ADN_MissionParameterType
    @brief  ADN_MissionParameterType
*/
// Created: SBO 2006-12-05
// =============================================================================
class ADN_MissionParameterType : public ADN_TableItem_ComboBox
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_MissionParameterType( ADN_Table* parent, void* data, T_ConnectorVector& itemConnectors );
    virtual ~ADN_MissionParameterType();
    //@}

    //! @name Operations
    //@{
    void Update();
    void Disconnect();
    //@}

signals:
    //! @name Signals
    //@{
    void TypeChanged( E_MissionParameterType type );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_MissionParameterType( const ADN_MissionParameterType& );            //!< Copy constructor
    ADN_MissionParameterType& operator=( const ADN_MissionParameterType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoValueChanged();
    //@}

private:
    //! @name Member data
    //@{
    T_ConnectorVector& itemConnectors_;
    //@}
};

#endif // __ADN_MissionParameterType_h_
