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

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

#include <qtable.h>

class QComboTableItem;

// =============================================================================
/** @class  ParamHumanWoundList
    @brief  ParamHumanWoundList
    @par    Using example
    @code
    ParamHumanWoundList;
    @endcode
*/
// Created: SBO 2005-09-27
// =============================================================================
class ParamHumanWoundList : public QTable, public Param_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ParamHumanWoundList );

public:
    //! @name Constructors/Destructor
    //@{
    explicit ParamHumanWoundList( ASN1T_SantePriorites& asnListHumanWound, const std::string& strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    virtual ~ParamHumanWoundList();
    //@}

    //! @name Operations
    //@{
    virtual void WriteMsg     ( std::stringstream& strMsg );
    virtual bool CheckValidity();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnHumanWoundChanged( int nRow, int nCol );
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_SantePriorites* pAsnHumanWoundList_;
    QStringList*          pHumanWoundsStringList_;
    //@}
};

#endif // __ParamHumanWoundList_h_
