// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Ghost_h_
#define __Ghost_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
    class GhostPrototype;
    class Entity_ABC;
    class SymbolFactory;
}

namespace gui
{
    class GLView_ABC;
    class Viewport_ABC;
}

namespace xml
{
    class xistream;
}

namespace tools
{
    class IdManager;
}

class LogisticBaseStates;
class Model;
class StaticModel;

// =============================================================================
/** @class  Ghost
    @brief  Ghost
*/
// Created: ABR 2011-10-14
// =============================================================================
class Ghost : public gui::EntityImplementation< kernel::Ghost_ABC >
            , public kernel::Extension_ABC
            , public kernel::Displayable_ABC
            , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Ghost( kernel::Controller& controller, const Model& model, tools::IdManager& idManager, const kernel::CoordinateConverter_ABC& converter, const kernel::GhostPrototype& prototype );
             Ghost( kernel::Controller& controller, const Model& model, tools::IdManager& idManager, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis, kernel::SymbolFactory& symbolsFactory );
             Ghost( kernel::Controller& controller, const Model& model, tools::IdManager& idManager, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis, kernel::Entity_ABC& parent, E_GhostType ghostType );
    virtual ~Ghost();
    //@}

    //! @name Ghost_ABC Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void ReadGhostAttributes( xml::xistream& xis );
    virtual void SerializeGhostAttributes( xml::xostream& xos ) const;
    virtual void Finalize( const StaticModel& staticModel );
    //@}

    //! @name Ghost_ABC Accessors
    //@{
    virtual const QString& GetType() const;
    virtual E_GhostType GetGhostType() const;
    virtual const std::string& GetLevel() const;
    virtual bool IsConverted() const;
    virtual const std::string& GetNature() const;
    virtual void UpdateSymbol( const std::string& level, const std::string& nature, const std::string& symbol );
    virtual const T_Children& GetChildren() const;
    //@}

    //! @name Operations
    //@{
    void InitializeSymbol() const;
    //@}

    //! @name Accessors
    //@{
    virtual const std::string& GetSymbol() const;
    virtual const std::string GetLevelSymbol() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, int >                    T_DotationsList;
    typedef T_DotationsList::const_iterator               CIT_DotationsList;

    typedef std::map< int, T_DotationsList >                T_DotationsMap;
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary();
    void ReadChildren( xml::xistream& xis );
    void ReadGhostSubordinate( xml::xistream& xis );
    void ReadGhostProfiles( xml::xistream& xis );
    void ReadDotations( xml::xistream& xis, int entityID );
    void ReadDotation( xml::xistream& xis, T_DotationsList& dotationList );
    void FinalizeDotations( const StaticModel& staticModel, const Entity_ABC&, LogisticBaseStates& logHierarchy );
    //@}

private:
    //! @name Member data
    //@{
    const Model&                            model_;
    const kernel::CoordinateConverter_ABC&  converter_;
    E_GhostType                             ghostType_;
    int                                     logisticSuperiorID_;
    std::vector< int >                      logisticSubordinatesID_;
    T_DotationsMap                          dotations_;
    T_Children                              children_;
    std::vector< std::string >              profilesReadOnly_;
    std::vector< std::string >              profilesWriteOnly_;
    QString                                 type_;
    mutable std::string                     symbol_; // $$$$ ABR 2011-11-07: Bad ... working on it
    std::string                             nature_;
    std::string                             level_;
    bool                                    converted_;
    //@}
};

#endif // __Ghost_h_
