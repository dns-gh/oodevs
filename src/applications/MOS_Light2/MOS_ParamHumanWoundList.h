// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_ParamHumanWoundList_h_
#define __MOS_ParamHumanWoundList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

#include <qtable.h>

class QComboTableItem;

// =============================================================================
/** @class  MOS_ParamHumanWoundList
    @brief  MOS_ParamHumanWoundList
    @par    Using example
    @code
    MOS_ParamHumanWoundList;
    @endcode
*/
// Created: SBO 2005-09-27
// =============================================================================
class MOS_ParamHumanWoundList : public QTable, public MOS_Param_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_ParamHumanWoundList );

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_ParamHumanWoundList( ASN1T_SantePriorites& asnListHumanWound, const std::string& strLabel, QWidget* pParent, bool bOptional );
    virtual ~MOS_ParamHumanWoundList();
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

#endif // __MOS_ParamHumanWoundList_h_
