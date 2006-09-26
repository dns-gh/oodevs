// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class InstanciationComplete;
}

class IdManager;

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: SBO 2006-09-19
// =============================================================================
class Formation : public kernel::Formation_ABC
                , public kernel::Extension_ABC
                , public kernel::Updatable_ABC< kernel::InstanciationComplete >
                , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Formation( kernel::Controller& controller, const QString& level, IdManager& idManager );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual bool IsInTeam( const kernel::Team_ABC& team ) const; // $$$$ AGE 2006-08-03: descendre dans Entity_ABC
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual const QString& GetLevel() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned long id_;
    QString name_;
    QString level_;
    //@}
};

#endif // __Formation_h_
