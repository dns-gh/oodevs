// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAtlasNature_h_
#define __ParamAtlasNature_h_

#include "Param_ABC.h"
#include "gaming/AtlasNature.h"

namespace kernel
{
    class OrderParameter;
}

class AtlasNatures;

// =============================================================================
/** @class  ParamAtlasNature
    @brief  ParamAtlasNature
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamAtlasNature : public QObject
                       , public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamAtlasNature( QObject* parent, const kernel::OrderParameter& parameter, const AtlasNatures& natures );
    virtual ~ParamAtlasNature();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ActionParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked( int id );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamAtlasNature( const ParamAtlasNature& );            //!< Copy constructor
    ParamAtlasNature& operator=( const ParamAtlasNature& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    void AddField( const QString& name, unsigned char byte, unsigned char mask );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const AtlasNature* > T_NatureFields;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    const AtlasNatures& natures_;
    AtlasNature nature_;
    T_NatureFields fields_;
    //@}
};

#endif // __ParamAtlasNature_h_
