// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAtlasNature_h_
#define __ParamAtlasNature_h_

#include "Param_ABC.h"
#include "clients_kernel/AtlasNature.h"

namespace kernel
{
    class OrderParameter;
    class AtlasNatures;
}

namespace actions
{
    namespace gui
    {

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
             ParamAtlasNature( QObject* parent, const kernel::OrderParameter& parameter, const kernel::AtlasNatures& natures );
    virtual ~ParamAtlasNature();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked( int id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamAtlasNature( const ParamAtlasNature& );            //!< Copy constructor
    ParamAtlasNature& operator=( const ParamAtlasNature& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    void AddField( const QString& name, unsigned char byte, unsigned char mask );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::AtlasNature* > T_NatureFields;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const kernel::AtlasNatures& natures_;
    kernel::AtlasNature nature_;
    T_NatureFields fields_;
    //@}
};

    }
}

#endif // __ParamAtlasNature_h_
