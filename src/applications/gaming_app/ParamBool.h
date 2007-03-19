// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamBool_h_
#define __ParamBool_h_

#include "Param_ABC.h"

// =============================================================================
/** @class  ParamBool
    @brief  ParamBool
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamBool : public QObject, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamBool( QObject* parent, const QString& name, bool defaultValue = false );
    virtual ~ParamBool();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( Action_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamBool( const ParamBool& );            //!< Copy constructor
    ParamBool& operator=( const ParamBool& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    bool value_;
    //@}
};

#endif // __ParamBool_h_
