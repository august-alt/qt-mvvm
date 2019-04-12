// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#include "viewmodelutils.h"
#include "model_types.h"
#include "sessionitem.h"
#include "sessionmodel.h"
#include "viewitem.h"
#include "customvariants.h"
#include <QStandardItemModel>

using namespace ModelView;

void Utils::iterate_model(const QStandardItemModel* model, const QModelIndex& parent,
                          std::function<void(QStandardItem*)> fun)
{
    if (!model)
        return;

    for (int row = 0; row < model->rowCount(parent); ++row) {
        for (int col = 0; col < model->columnCount(parent); ++col) {
            auto index = model->index(row, col, parent);
            auto item = model->itemFromIndex(index);
            if (item)
                fun(item);
        }
        for (int col = 0; col < model->columnCount(parent); ++col) {
            auto index = model->index(row, col, parent);
            iterate_model(model, index, fun);
        }
    }
}

std::vector<ViewItem*> Utils::findViews(const QStandardItemModel* model, const QModelIndex& parent,
                                        const ModelView::SessionItem* item)
{
    std::vector<ModelView::ViewItem*> result;
    iterate_model(model, parent, [&](QStandardItem* standard_item) {
        if (auto view = dynamic_cast<ViewItem*>(standard_item)) {
            if (view->item() == item)
                result.push_back(view);
        }
    });

    return result;
}

//! Translates SessionItem's data role to vector of Qt roles.

QVector<int> Utils::item_role_to_qt(int role)
{
    QVector<int> result;
    // In Qt when we are editing the data in a view two roles are emmited.
    if (role == ItemDataRole::DISPLAY || role == ItemDataRole::DATA)
        result = {Qt::DisplayRole, Qt::EditRole};
    else if (role == ItemDataRole::APPEARANCE)
        result = {Qt::TextColorRole};

    return result;
}

QVariant Utils::TextColorRole(const SessionItem& item)
{
    return item.isEnabled() ? QVariant() : QColor(Qt::gray);
}

QVariant Utils::CheckStateRole(const SessionItem& item)
{
    auto value = item.data(ItemDataRole::DATA);
    if (Utils::IsBoolVariant(value))
        return value.toBool() ? Qt::Checked : Qt::Unchecked;
    return QVariant();
}
