// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgentType_h_
#define __ParamAgentType_h_

#include "Param_ABC.h"

// =============================================================================
/** @class  ParamAgentType
    @brief  ParamAgentType
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamAgentType : public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ParamAgentType( const QString& name );
    virtual ~ParamAgentType();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamAgentType( const ParamAgentType& );            //!< Copy constructor
    ParamAgentType& operator=( const ParamAgentType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    QCheckBox* pArmoredCheckBox_;
    QCheckBox* pASSCheckBox_;
    QCheckBox* pHQCheckBox_;
    QCheckBox* pLogCheckBox_;
    QCheckBox* pMortarCheckBox_;
    QCheckBox* pRocketLauncherCheckBox_;
    QCheckBox* pVehiculeCheckBox_;
    QCheckBox* pFootTroopsCheckBox_;
    QCheckBox* pHelicopterCheckBox_;
    QCheckBox* pUndefinedCheckBox_;
    QCheckBox* pNoneCheckBox_;
    //@}
};

#endif // __ParamAgentType_h_
