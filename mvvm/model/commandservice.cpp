// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#include "commandservice.h"
#include "commands.h"
#include "sessionmodel.h"
#include "sessionitem.h"
#include <QUndoStack>

CommandService::CommandService(SessionModel* model)
    : m_model(model)
{

}

void CommandService::setUndoRedoEnabled(bool value)
{
    if (value)
        m_commands.reset(new QUndoStack);
    else
        m_commands.reset();
}

void CommandService::push(QUndoCommand* command)
{
    if (provideUndo()) {
        m_commands->push(command);
    } else {
        command->redo();
        delete command;
    }
}

SessionItem* CommandService::insertNewItem(const model_type& modelType, SessionItem* parent, int row)
{
    if (!parent)
        parent = m_model->rootItem();

    push(new InsertNewItemCommand(modelType, parent, row));

    return parent->childAt(row);
}

bool CommandService::provideUndo() const
{
    return m_commands ? true : false;
}
