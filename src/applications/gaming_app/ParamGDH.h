// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamGDH_h_
#define __ParamGDH_h_

#include "game_asn/Asn.h"
#include "Param_ABC.h"

// =============================================================================
/** @class  ParamGDH
    @brief  ParamGDH
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamGDH : public QObject, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamGDH( QWidget* parent, ASN1T_GDH*& asn, const QString& label );
    virtual ~ParamGDH();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCheckboxToogled( bool );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamGDH( const ParamGDH& );            //!< Copy constructor
    ParamGDH& operator=( const ParamGDH& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_GDH*     asn_;
    QDateTimeEdit* pDateTimeEdit_;
    QCheckBox*     pCheckbox_;
    //@}
};

#endif // __ParamGDH_h_
