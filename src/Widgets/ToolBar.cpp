#include "includes/Widgets/ToolBar.hpp"
#include "includes/MainWindow.hpp"

#include <QInputDialog>

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
	itemInToolBar.push_back(action);
	addAction(action);
}

void ToolBar::addNewWidget(QWidget * widget)
{
	itemInToolBar.push_back(widget);
	addWidget(widget);
}

void ToolBar::reset()
{
	itemInToolBar.clear();
	clear();

	addAction(m_paramAction);
	addAction(m_removeAction);
	addSeparator();
	addSeparator();
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

	for (int i{ 0 }; i < m_parentToolBar->itemInToolBar.size(); ++i) {
		QString actionName{ m_parentToolBar->itemInToolBar[i]->objectName() };
		QIcon actionIcon{};
		if (m_parentToolBar->itemInToolBar[i]->objectName() != "searchArea" || m_parentToolBar->itemInToolBar[i]->objectName() != "urlArea")
			actionIcon = static_cast<QAction*>(m_parentToolBar->itemInToolBar[i])->icon();
		m_model->appendRow(new QStandardItem(actionIcon, actionName));
	}

	m_layout->addWidget(m_view);
	m_layout->addLayout(m_buttonLayout);

	m_buttonLayout->addWidget(m_newButton);
	m_buttonLayout->addWidget(m_deleteButton);
	m_buttonLayout->addWidget(m_boxBtn);

	connect(m_newButton, &QPushButton::clicked, this, &ManageToolBar::newItem);
	connect(m_boxBtn, &QDialogButtonBox::accepted, this, &ManageToolBar::accept);
	connect(m_boxBtn, &QDialogButtonBox::rejected, this, &ManageToolBar::close);
}

ManageToolBar::~ManageToolBar()
{
	// Empty
}

void ManageToolBar::newItem()
{
	QStringList items{};

	QHashIterator<QString, QAction*> i{ m_parent->m_editableAction };
	while (i.hasNext())
	{
		i.next();
		items << i.key();
	}
	items << "urlArea" << "searchArea";

	bool ok{ false };
	QString newAction{ QInputDialog::getItem(this, tr("Action � ajouter"), tr("Actions : "), items, 0, false, &ok) };

	if (ok && !newAction.isEmpty()) {
		if (newAction != "urlArea" && newAction != "searchArea") {
			QString actionName{ m_parent->m_editableAction[newAction]->objectName() };
			QIcon actionIcon{ m_parent->m_editableAction[newAction]->icon() };
			m_model->appendRow(new QStandardItem(actionIcon, actionName));
		}
		else if (newAction == "urlArea")
			m_model->appendRow(new QStandardItem("urlArea"));
		else if (newAction == "searchArea")
			m_model->appendRow(new QStandardItem("searchArea"));
	}
}

void ManageToolBar::accept()
{
	m_parentToolBar->reset();

	for (int i{ 0 }; i < m_model->rowCount(); ++i) {
		if (m_model->item(i)->text() != "urlArea" && m_model->item(i)->text() != "searchArea")
			m_parentToolBar->addNewAction(m_parent->m_editableAction[m_model->item(i)->text()]);
		else if (m_model->item(i)->text() == "urlArea")
			m_parentToolBar->addNewWidget(m_parent->m_urlArea);
		else if (m_model->item(i)->text() == "searchArea")
			m_parentToolBar->addNewWidget(m_parent->m_searchArea);
	}

	close();
}

Qt::DropActions QStandardItemModel::supportedDropActions() const
{
	return Qt::MoveAction;
}


