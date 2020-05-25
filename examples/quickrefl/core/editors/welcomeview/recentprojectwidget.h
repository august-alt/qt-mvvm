// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef RECENTPROJECTWIDGET_H
#define RECENTPROJECTWIDGET_H

#include <QWidget>
#include <memory>
#include <vector>

class QBoxLayout;
class ProjectPaneWidget;

//! Widget with the name of current project and tree of recent projects.
//! Occupies left part of WelcomeView.

class RecentProjectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RecentProjectWidget(QWidget* parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void setCurrentProject(const std::string& project_title, const std::string& project_dir);

    void setRecentProjectsList(const QStringList& projects);

private:
    QBoxLayout* createCurrentProjectLayout() const;
    QBoxLayout* createRecentProjectLayout();
    ProjectPaneWidget* m_current_project_pane;
    std::vector<ProjectPaneWidget*> m_recent_project_panes;

};

#endif // RECENTPROJECTWIDGET_H