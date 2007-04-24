// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamDirection_h_
#define __ParamDirection_h_

#include "game_asn/Asn.h"
#include "Param_ABC.h"

// =============================================================================
/** @class  ParamDirection
    @brief  ParamDirection
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamDirection : public QObject, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamDirection( QObject* parent, const QString& name );
    virtual ~ParamDirection();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( ASN1T_OrderContext& asn ) const;
    virtual void CommitTo( Action_ABC& action, bool context ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( int value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamDirection( const ParamDirection& );            //!< Copy constructor
    ParamDirection& operator=( const ParamDirection& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    int value_;
    //@}
};


#endif // __ParamDirection_h_
