// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#ifndef CUSTOMVARIANTS_H
#define CUSTOMVARIANTS_H

//! Registrations for custom variants.

#include <QVariant>
#include <QMetaType>
#include <string>
#include <vector>

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(std::vector<double>)

#endif
