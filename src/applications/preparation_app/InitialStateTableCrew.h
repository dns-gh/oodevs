// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InitialStateTableCrew_h_
#define __InitialStateTableCrew_h_

#include "InitialStateTable_ABC.h"
#include "preparation/InitialStateCrew.h"

class InitialState;

// =============================================================================
/** @class  InitialStateTableCrew
    @brief  InitialStateTableCrew
*/
// Created: ABR 2011-02-24
// =============================================================================
class InitialStateTableCrew : public InitialStateTable_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InitialStateTableCrew( QWidget* parent, const char* name = 0 );
    virtual ~InitialStateTableCrew();
    //@}

    //! @name Operations
    //@{
    virtual void ReadExtension( const InitialState& extension );
    virtual void WriteExtension( InitialState& extension ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Column { eRank = 0, eState = 1, ePsy = 2, eContaminated = 3, eNumber = 4 };
    //@}

    //! @name Helpers
    //@{
    virtual void paintCell( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg );
    virtual void keyPressEvent( QKeyEvent * e );
    void AddHeader( InitialStateCrew::E_CrewRanks rank, int number );
    void AddLine( InitialStateCrew::E_CrewRanks rank = InitialStateCrew::eOfficer, InitialStateCrew::E_CrewStates state = InitialStateCrew::eHealthy, bool psy = false, bool contaminated = false, int number = 0 );
    void ComputeValues( InitialStateCrew::E_CrewRanks rank );
    void ComputeAllValues();
    //@}

private slots:
    void OnAddItem();
    void OnClearItems();
    void OnRemoveItem();
    void OnRequestContextMenu( int row, int col, const QPoint& pos );
    void OnValueChanged( int row, int col );

private:
    //! @name Member data
    //@{
    QStringList ranks_;
    QStringList states_;
    std::map< InitialStateCrew::E_CrewRanks, int > nbrOfficers_;
    std::map< InitialStateCrew::E_CrewRanks, int > nbrOfficersLeft_;
    //@}
};

#endif // __InitialStateTableCrew_h_
