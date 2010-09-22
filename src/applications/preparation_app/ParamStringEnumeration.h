// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ParamStringEnumeration_h_
#define __ParamStringEnumeration_h_

#include "actions_gui/Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

class StringQVButtonGroup;

// =============================================================================
/** @class  ParamStringEnumeration
    @brief  ParamStringEnumeration
*/
// Created: SBO 2009-08-05
// =============================================================================
class ParamStringEnumeration : public QObject, public actions::gui::Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    ParamStringEnumeration( QObject* parent, const QString& title, const kernel::OrderParameter& parameter, const std::map< std::string, std::string >& values );
    virtual ~ParamStringEnumeration();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual bool CheckValidity();
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnToggle( const std::string& id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamStringEnumeration( const ParamStringEnumeration& );            //!< Copy constructor
    ParamStringEnumeration& operator=( const ParamStringEnumeration& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddItem( StringQVButtonGroup* group, const std::pair<std::string, std::string>& name );
    QString GetValue() const;
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, bool > T_Items;
    typedef std::map< std::string, std::string >::const_iterator CIT_Values;
    //@}

private:
    //! @name Member data
    //@{
    const QString title_;
    const kernel::OrderParameter parameter_;
    const std::map< std::string, std::string > values_;
    T_Items selected_;
    //@}
};

#endif // __ParamStringEnumeration_h_
