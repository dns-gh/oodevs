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

#include "Param_ABC.h"

namespace kernel
{
    class OrderParameter;
}

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
             ParamHumanWoundList( QObject* parent, const kernel::OrderParameter& parameter );
    virtual ~ParamHumanWoundList();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnHumanWoundChanged( int row, int col );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    QTable*     table_;
    QStringList humanWoundsList_;
    //@}
};

#endif // __ParamHumanWoundList_h_
