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
             ParamGDH( QObject* parent, const QString& name );
    virtual ~ParamGDH();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( Action_ABC& action, bool context ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEnabled( bool );
    void OnValueChanged( const QDateTime& datetime );
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
    QDateTime datetime_;
    bool valueSet_;
    //@}
};

#endif // __ParamGDH_h_
