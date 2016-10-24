#include "includes/Widgets/ToolBar.hpp"
#include "includes/MainWindow.hpp"

ToolBar::ToolBar(MainWindow * parent) : 
	QToolBar(parent),
	m_parent(parent)
{
	addAction(m_paramAction);
	addAction(m_removeAction);
	addSeparator();
	addSeparator();

	connect(m_paramAction, &QAction::triggered, this, &ToolBar::openParams);
}

ToolBar::~ToolBar()
{
	// Empty
}

void ToolBar::addNewAction(QAction *action)
{
	actionsInToolBar.push_back(action);
	addAction(action);
}

void ToolBar::reset()
{

}

void ToolBar::openParams()
{
	ManageToolBar *manager{ new ManageToolBar(this, m_parent) };
	manager->show();
}

ManageToolBar::ManageToolBar(ToolBar * parentToolBar, MainWindow * parent) : 
	QDialog(parent),
	m_parent(parent),
	m_parentToolBar(parentToolBar)
{
	m_view->setAcceptDrops(true);
	m_view->setDragEnabled(true);
	m_view->setDropIndicatorShown(true);
	m_view->setModel(m_model);

	for (int i{ 0 }; i < m_parentToolBar->actionsInToolBar.size(); ++i) {
		QString actionName{ m_parentToolBar->actionsInToolBar[i]->objectName() };
		QIcon actionIcon{ m_parentToolBar->actionsInToolBar[i]->icon() };
		m_model->appendRow(new QStandardItem(actionIcon, actionName));
	}

	m_layout->addWidget(m_view);
	m_layout->addLayout(m_buttonLayout);

	m_buttonLayout->addWidget(m_newButton);
	m_buttonLayout->addWidget(m_deleteButton);
	m_buttonLayout->addWidget(m_boxBtn);
}

ManageToolBar::~ManageToolBar()
{
	// Empty
}

void ManageToolBar::accept()
{
}

Qt::DropActions QStandardItemModel::supportedDropActions() const
{
	return Qt::MoveAction;
}


