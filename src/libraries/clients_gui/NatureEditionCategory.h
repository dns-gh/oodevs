// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NatureEditionCategory_h_
#define __NatureEditionCategory_h_

#include "clients_kernel/SymbolVisitor_ABC.h"

namespace gui
{

// =============================================================================
/** @class  NatureEditionCategory
    @brief  NatureEditionCategory
*/
// Created: AGE 2006-10-24
// =============================================================================
class NatureEditionCategory : public QObject
                            , public kernel::SymbolVisitor_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             NatureEditionCategory( QGridLayout* parent, int row, int deep = -1 );
    virtual ~NatureEditionCategory();
    //@}

    //! @name Operations
    //@{
    QString GetNature() const;
    void SetNature( const QString& nature );
    void SetRootSymbolRule( kernel::SymbolRule& root );
    void Clear();
    //@}

signals:
    //! @name Signals
    //@{
    void NatureChanged( const QString& nature );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnComboChange();
    void OnNatureChanged( const QString& nature );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    NatureEditionCategory( const NatureEditionCategory& );            //!< Copy constructor
    NatureEditionCategory& operator=( const NatureEditionCategory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void StartCategory( const std::string& title );
    virtual void AddChoice( kernel::SymbolRule* rule, const std::string& name, const std::string& value );
    virtual void EndCategory();
    void Select( const QString& value );
    //@}

private:
    //! @name Member data
    //@{
    QGridLayout*                     parentLayout_;
    int                              row_;
    int                              deep_;
    QLabel*                          label_;
    QComboBox*                       box_;
    const kernel::SymbolRule*        rule_;
    std::map< QString, std::string > internalNames_;
    QString                          current_;
    NatureEditionCategory*           next_;
    //@}
};

}

#endif // __NatureEditionCategory_h_
