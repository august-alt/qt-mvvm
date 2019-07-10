// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#ifndef MVVM_ABSTRACTITEMCOMMAND_H
#define MVVM_ABSTRACTITEMCOMMAND_H

#include "mvvm_global.h"
#include "path.h"
#include <string>

namespace ModelView
{

class SessionItem;
class SessionModel;

//! Abstract command interface to manipulate SessionItem in model context.

class CORE_EXPORT AbstractItemCommand
{
public:
    explicit AbstractItemCommand(SessionItem* receiver);
    virtual ~AbstractItemCommand() = default;

    void execute();

    void undo();

    bool isObsolete() const;

    std::string description() const;

protected:
    void setObsolete(bool flag);
    void setDescription(const std::string& description);
    Path pathFromItem(SessionItem* item) const;
    SessionItem* itemFromPath(Path path) const;
    SessionModel* model() const;

private:
    virtual void execute_command() = 0;
    virtual void undo_command() = 0;

    bool m_is_obsolete;
    std::string m_text;
    SessionModel* m_model;
};

} // namespace ModelView

#endif // MVVM_ABSTRACTITEMCOMMAND_H