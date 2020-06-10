// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef PROJECTHANDLER_H
#define PROJECTHANDLER_H

#include <QObject>
#include <memory>
#include <vector>
#include <mvvm/interfaces/applicationmodelsinterface.h>

namespace ModelView
{
class ProjectManagerInterface;
}

class RecentProjectSettings;
class UserInteractor;
class SampleModel;

//! Main class to coordinate all activity on user's request to create new project,
//! open existing one, or choose one of recent projects on disk.

// FIXME remove multiple inheritance when callback in Document will be implemented

class ProjectHandler : public QObject, public ModelView::ApplicationModelsInterface
{
    Q_OBJECT
public:
    explicit ProjectHandler(SampleModel* sample_model, QObject* parent = nullptr);
    ~ProjectHandler() override;

    std::vector<ModelView::SessionModel*> persistent_models() const override;

public slots:
    void onCreateNewProject();
    void onOpenExistingProject(const QString& dirname = {});
    void onSaveCurrentProject();
    void onSaveProjectAs();

private:
    void init_project_manager();
    void update_current_project_name();
    void update_recent_project_names();

    std::unique_ptr<RecentProjectSettings> m_recentProjectSettings;
    std::unique_ptr<UserInteractor> m_userInteractor;
    std::unique_ptr<ModelView::ProjectManagerInterface> m_projectManager;
    SampleModel* m_model{nullptr};
};

#endif // PROJECTHANDLER_H
