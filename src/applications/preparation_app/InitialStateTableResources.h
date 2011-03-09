// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InitialStateTableResources_h_
#define __InitialStateTableResources_h_

#include "InitialStateTable_ABC.h"

class StaticModel;
class InitialState;

// =============================================================================
/** @class  InitialStateTableResources
    @brief  InitialStateTableResources
*/
// Created: ABR 2011-02-24
// =============================================================================
class InitialStateTableResources : public InitialStateTable_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InitialStateTableResources( QWidget* parent, const StaticModel& staticModel, const char* name = 0 );
    virtual ~InitialStateTableResources();
    //@}

    //! @name Operations
    //@{
    virtual void ReadExtension( const InitialState& extension );
    virtual void WriteExtension( InitialState& extension ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Column { eName = 0, eCategory = 1, eNumber = 2, eThreshold = 3 };
    //@}

    //! @name Helpers
    //@{
    virtual void keyPressEvent( QKeyEvent * e );
    void OnAddItem( int id );
    void AddLine( const QString& name, const QString& category, unsigned number = 0, double threshold = 0. );
    bool IsDotationAlreadyPresent( const QString& name ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRemoveCurrentItem();
    void OnRequestContextMenu( int row, int col, const QPoint& pos );
    //@}

private:
    //! @name Member data
    //@{
    const StaticModel& staticModel_;
    //@}
};

#endif // __InitialStateTableResources_h_
