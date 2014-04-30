// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_UnitStateTableCrew_h_
#define __gui_UnitStateTableCrew_h_

#include "UnitStateTable_ABC.h"

namespace gui
{
// =============================================================================
/** @class  UnitStateTableCrew
    @brief  UnitStateTableCrew
*/
// Created: ABR 2011-07-05
// =============================================================================
class UnitStateTableCrew : public UnitStateTable_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit UnitStateTableCrew( const QString& objectName,
                                 QWidget* parent,
                                 kernel::Controllers& controllers );
    virtual ~UnitStateTableCrew();
    //@}

public:
    //! @name Operations
    //@{
    virtual bool IsReadOnlyForType( const std::string& typeName ) const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_Column { eRank = 0, eState = 1, eInjuries = 2, eLocation = 3, ePsy = 4, eContaminated = 5, eNumber = 6 };
    //@}

    //! @name Helpers
    //@{
    virtual void Purge();
    void MergeLine( E_HumanRank rank, E_HumanState state, E_InjuriesSeriousness seriousness, bool psy, bool contaminated, int number, E_HumanLocation location = eHumanLocation_Battlefield );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* e );
    virtual void keyPressEvent( QKeyEvent * e );
    void ComputeValues( E_HumanRank rank );
    void ComputeAllValues();
    //@}

private slots:
    //! @name Slots
    //@{
    void AddLine( int number = 0, E_HumanRank rank = eHumanRank_Officier, E_HumanState state = eHumanState_Healthy, E_InjuriesSeriousness seriousness = eInjuriesSeriousness_U1,
                  bool psy = false, bool contaminated = false, E_HumanLocation location = eHumanLocation_Battlefield );
    void OnClearItems();
    void OnRemoveCurrentItem();
    void OnItemChanged( QStandardItem* item );
    //@}

private:
    //! @name Member data
    //@{
    std::map< E_HumanRank, int > nbrOfficersTotal_;
    std::map< E_HumanRank, int > nbrOfficersHealthy_;
    bool                         updating_;
    //@}
};

}

#endif // __gui_UnitStateTableCrew_h_
