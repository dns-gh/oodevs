// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionInterface_ABC_h_
#define __MissionInterface_ABC_h_

#include "network/Simulation_Asn.h"
#include "ParamComboBox.h"
#include "ParamRadioBtnGroup.h"

namespace kernel
{
    class ActionController;
    class Entity_ABC;
}

class Param_ABC;
class OptionalParamFunctor_ABC;

// =============================================================================
/** @class  MissionInterface_ABC
    @brief  MissionInterface_ABC
*/
// Created: APE 2004-04-20
// =============================================================================
class MissionInterface_ABC : public QVBox
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             MissionInterface_ABC( QWidget* parent, kernel::Entity_ABC& agent, kernel::ActionController& controller );
    virtual ~MissionInterface_ABC();
    //@}

    //! @name Operations
    //@{
    bool CheckValidity();
    void Draw( const kernel::GlTools_ABC& tools, const geometry::Rectangle2f& extent ) const;
    bool IsEmpty() const;
    void AddParameter( Param_ABC& parameter, OptionalParamFunctor_ABC* pOptional );
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void OnOk() = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    void CreateOkCancelButtons();
    
//    template < class T >
//    ParamComboBox< T >& CreateVarList( T& value, const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 )
//    {
//        ParamComboBox< T >* pParam = new ParamComboBox<T>( this, value, strName );
//        AddParameter( *pParam, pOptional  );
//        return *pParam;
//    }
//
//    template < class T >
//    ParamRadioBtnGroup< T >& CreateRadioButtonGroup( T& value, const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 )
//    {
//        ParamRadioBtnGroup< T >* pParam = new ParamRadioBtnGroup<T>( this, value, strName );
//        AddParameter( *pParam, pOptional );
//        return *pParam;
//    }
//
//    template< typename T, typename U > T* BuildOptionalParamFunctor( U& asnMission )
//    {
//        T* result = new T( asnMission );
//        optionalFunctors_.push_back( result );
//        return result;
//    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionInterface_ABC( const MissionInterface_ABC& );
    MissionInterface_ABC& operator=( const MissionInterface_ABC& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< Param_ABC* >      T_Parameters;
    typedef T_Parameters::const_iterator CIT_Parameters;

    typedef std::vector< OptionalParamFunctor_ABC* >  T_OptionalFunctors;
    typedef T_OptionalFunctors::const_iterator      CIT_OptionalFunctors;
    //@}

protected:
    //! @name Helpers
    //@{
    void Commit();
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController& controller_;
    kernel::Entity_ABC& agent_;

    T_Parameters  parameters_;
    T_OptionalFunctors optionalFunctors_;
};

#endif // __MissionInterface_ABC_h_
