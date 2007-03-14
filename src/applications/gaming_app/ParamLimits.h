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
#include "game_asn/Asn.h"

class LimitParameter;
class Limit;

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
             ParamLimits( QObject* parent, ASN1T_Line& limit1, ASN1T_Line& limit2, const QString& name1, const QString& name2, const QString& menu1, const QString& menu2 );
    virtual ~ParamLimits();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void SetOptional( OptionalParamFunctor_ABC* functor );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual bool CheckValidity();
    virtual void BuildInterface( QWidget* parent );
    virtual void Commit();
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
