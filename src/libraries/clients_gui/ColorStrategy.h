// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColorStrategy_h_
#define __ColorStrategy_h_

#include "ColorStrategy_ABC.h"
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
    class Team_ABC;
    class Automat_ABC;
    class Controllers;
    class GlTools_ABC;
    class TacticalLine_ABC;
    class Knowledge_ABC;
}

namespace gui
{

// =============================================================================
/** @class  ColorStrategy
    @brief  ColorStrategy
*/
// Created: AGE 2006-03-17
// =============================================================================
class ColorStrategy : public ColorStrategy_ABC
                    , public kernel::Observer_ABC
                    , public kernel::ElementObserver_ABC< kernel::Team_ABC >
                    , public kernel::SelectionObserver_ABC
                    , public kernel::SelectionObserver_Base< kernel::Agent_ABC >
                    , public kernel::SelectionObserver_Base< kernel::Object_ABC >
                    , public kernel::SelectionObserver_Base< kernel::Population_ABC >
                    , public kernel::SelectionObserver_Base< kernel::TacticalLine_ABC >
                    , public kernel::SelectionObserver_Base< kernel::Knowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ColorStrategy( kernel::Controllers& controllers, kernel::GlTools_ABC& tools );
    virtual ~ColorStrategy();
    //@}

    //! @name Operations
    //@{
    virtual void SelectColor( const kernel::Agent_ABC& agent );
    virtual void SelectColor( const kernel::Object_ABC& object );
    virtual void SelectColor( const kernel::Population_ABC& population );
    virtual void SelectColor( const kernel::Knowledge_ABC& knowledge );
    virtual void SelectColor( const kernel::TacticalLine_ABC& line );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ColorStrategy( const ColorStrategy& );            //!< Copy constructor
    ColorStrategy& operator=( const ColorStrategy& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& );
    virtual void NotifyUpdated( const kernel::Team_ABC& );
    virtual void NotifyDeleted( const kernel::Team_ABC& );

    virtual void BeforeSelection();
    virtual void Select( const kernel::Agent_ABC& element );
    virtual void Select( const kernel::Object_ABC& element );
    virtual void Select( const kernel::Population_ABC& element );
    virtual void Select( const kernel::TacticalLine_ABC& element );
    virtual void Select( const kernel::Knowledge_ABC& element );
    virtual void AfterSelection();

    void InitializeSynonyms();
    void InitializeColors();

    std::string RandomName() const;
    QColor RandomColor() const;
    void CreateNewColor( const std::string& name );
    QColor SelectedColor( const QColor& base ) const;
    QColor SuperiorSelectedColor( const QColor& base ) const;
    QColor KnowledgeColor( const QColor& base ) const;
    void ApplyColor( const QColor& color ) const;
    QColor FindColor( const kernel::Entity_ABC& entity );
    //@}

    //! @name Types 
    //@{
    typedef std::map< const kernel::Entity_ABC*, std::pair< std::string, QColor > > T_TeamColors;
    typedef std::map< std::string, std::string >                                    T_Synonyms;
    typedef std::map< std::string, QColor >                                         T_Colors;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::GlTools_ABC& tools_;

    kernel::SafePointer< kernel::Object_ABC >       selectedObject_;
    kernel::SafePointer< kernel::Agent_ABC >        selectedAgent_;
    kernel::SafePointer< kernel::Population_ABC >   selectedPopulation_;
    kernel::SafePointer< kernel::TacticalLine_ABC > selectedLine_;
    kernel::SafePointer< kernel::Knowledge_ABC >    selectedKnowledge_;

    T_TeamColors teamColors_;
    T_Synonyms   synonyms_;
    T_Colors     available_;
    //@}
};

}

#endif // __ColorStrategy_h_
