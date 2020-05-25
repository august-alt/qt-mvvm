// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_WIDGETS_UTILS_H
#define MVVM_WIDGETS_UTILS_H

#include <mvvm/core/export.h>

class QColor;
class QString;

namespace ModelView
{

//! Collection of various widget-related utils.

namespace Utils
{

//! Returns random color.
CORE_EXPORT QColor random_color();

//! Returns true if it is Windows.
CORE_EXPORT bool IsWindowsHost();

//! Returns a string where path to the file striped using '~'.
CORE_EXPORT QString WithTildeHomePath(const QString& path);

} // namespace Utils

} // namespace ModelView

#endif // MVVM_WIDGETS_UTILS_H