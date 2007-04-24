// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLimits_h_
#define __ParamLimits_h_

#include "Param_ABC.h"

class LimitParameter;

// =============================================================================
/** @class  ParamLimits
    @brief  ParamLimits
*/
// Created: AGE 2006-03-24
// =============================================================================
class ParamLimits : public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamLimits( QObject* parent, const QString& name1, const QString& name2 );
    virtual ~ParamLimits();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void SetOptional( bool optional );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual bool CheckValidity();
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ASN1T_OrderContext& asn ) const;
    virtual void Clean( ASN1T_OrderContext& asn ) const;
    virtual void CommitTo( Action_ABC& action, bool context ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamLimits( const ParamLimits& );            //!< Copy constructor
    ParamLimits& operator=( const ParamLimits& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    LimitParameter* limit1_;
    LimitParameter* limit2_;
    //@}
};

#endif // __ParamLimits_h_
