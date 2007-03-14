// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamRadioBtnGroup_h_
#define __ParamRadioBtnGroup_h_

#include "Param_ABC.h"
#include <qhbuttongroup.h>

// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
template< typename T >
class ParamRadioBtnGroup : public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamRadioBtnGroup( T& output, const QString& name );
    virtual ~ParamRadioBtnGroup();
    //@}

    //! @name Operations
    //@{
    void AddButton( const QString& label, const T& value, bool selected = false );
    virtual void Commit();
    virtual void BuildInterface( QWidget* parent );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamRadioBtnGroup( const ParamRadioBtnGroup& );
    ParamRadioBtnGroup& operator=( const ParamRadioBtnGroup& );
    //@}

private:
    //! @name Member data
    //@{
    QHButtonGroup* group_;
    T& output_;
    std::vector< T > values_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< typename T >
ParamRadioBtnGroup<T>::ParamRadioBtnGroup( T& output, const QString& name )
    : Param_ABC( name )
    , group_( 0 )
    , output_( output )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< typename T >
ParamRadioBtnGroup<T>::~ParamRadioBtnGroup()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
template< typename T >
void ParamRadioBtnGroup<T>::BuildInterface( QWidget* parent )
{
    group_ = new QHButtonGroup( GetName(), parent );
}

// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup::AddButton
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
template< typename T >
void ParamRadioBtnGroup<T>::AddButton( const QString& label, const T& value, bool selected /*= false*/ )
{
    if( !group_ )
        InterfaceNotInitialized();
    QRadioButton* button = new QRadioButton( label, group_, 0 );
    button->setChecked( values_.empty() || selected );
    values_.push_back( value );
}

// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup::Commit
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< typename T >
void ParamRadioBtnGroup<T>::Commit()
{
    if( !group_ )
        InterfaceNotInitialized();
    output_ = values_.at( group_->selectedId() );
}

#endif // __ParamRadioBtnGroup_h_
