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

class ADN_Missions_Parameter;

// =============================================================================
/** @class  ADN_MissionParameters_Table
    @brief  ADN_MissionParameters_Table
*/
// Created: SBO 2006-12-04
// =============================================================================
class ADN_MissionParameters_Table : public ADN_Table
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_MissionParameters_Table( const QString& objectName, ADN_Connector_ABC*& connector, E_MissionType missionType, QWidget* pParent = 0 );
    virtual ~ADN_MissionParameters_Table();
    //@}

    //! @name Operations
    //@{
    void SetItemConnectors( const T_ConnectorVector& itemConnectors );
    //@}

    //! @name ADN_Table Re-implementation
    //@{
    virtual void AddRow( int row, void* data );
    virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
    virtual void OnContextMenu( const QPoint& pt );
    //@}

private:
    //! @name Helpers
    //@{
    void AddNewElement();
    virtual void RemoveCurrentElement();
    void ResetCurrent();
    void Reconnect( const QModelIndex& index );
    void Disconnect( ADN_Missions_Parameter* param );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( const QItemSelection&, const QItemSelection& );
    void OnMissionSelectionChanged();
    //@}

signals:
    //! @name Signals
    //@{
    void TypeChanged( E_MissionParameterType type );
    //@}

private:
    //! @name Member data
    //@{
    T_ConnectorVector itemConnectors_;
    QStringList parameterTypes_;
    bool addingRow_;
    E_MissionType missionType_;
    //@}
};

#endif // __ADN_MissionParameters_Table_h_
