// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#ifndef SESSIONMODEL_H
#define SESSIONMODEL_H

#include <QVariant>
#include <memory>
#include <string>
#include "path.h"
#include "mvvm_types.h"

class SessionItem;
class QUndoStack;
class ItemFactory;
class CommandService;

class SessionModel
{
public:
    SessionModel(const std::string& model_type = {});
    virtual ~SessionModel();

    std::string modelType() const;

    SessionItem* insertNewItem(const model_type& modelType, SessionItem* parent = nullptr,
                               int row = -1);

    SessionItem* rootItem() const;

    QVariant data(SessionItem* item, int role) const;

    bool setData(SessionItem* item, const QVariant& value, int role);

    Path pathFromItem(SessionItem* item);
    SessionItem* itemFromPath(Path path);

    void setUndoRedoEnabled(bool value);
    void setUndoRecordPause(bool value);

    QUndoStack *undoStack() const;

    ItemFactory* factory();

    void removeRow(SessionItem* parent, int row);

    void insertRow(SessionItem* parent, int row, SessionItem* child);

private:
    void createRootItem();
    bool provideUndo() const;

    SessionItem* m_root_item;
    std::unique_ptr<QUndoStack> m_undoStack;
    std::unique_ptr<CommandService> m_commands;
    std::unique_ptr<ItemFactory> m_item_factory;
    std::string m_model_type;
    bool m_pause_undo;
};

#endif
