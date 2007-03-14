// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamHumanWoundList_h_
#define __ParamHumanWoundList_h_

#include "game_asn/Asn.h"
#include "Param_ABC.h"

#include <qtable.h>

// =============================================================================
/** @class  ParamHumanWoundList
    @brief  ParamHumanWoundList
*/
// Created: SBO 2005-09-27
// =============================================================================
class ParamHumanWoundList : public QObject, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamHumanWoundList( QObject* parent, ASN1T_SantePriorites*& asnListHumanWound, const QString& name );
    virtual ~ParamHumanWoundList();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void Commit();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnHumanWoundChanged( int row, int col );
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_SantePriorites* pAsnHumanWoundList_;
    QTable*               table_;
    QStringList           humanWoundsList_;
    //@}
};

#endif // __ParamHumanWoundList_h_
