#include "includes/Widgets/ToolBar.hpp"
#include "includes/MainWindow.hpp"

#include <QInputDialog>
#include <QMessageBox>
#include <QString>
#include <QTextStream>

ToolBar::ToolBar(MainWindow * parent) : 
	QToolBar(parent),
	m_parent(parent)
{
	addAction(m_paramAction);
	addAction(m_removeAction);
	addSeparator();
	addSeparator();

	connect(m_paramAction, &QAction::triggered, this, &ToolBar::openParams);
	connect(m_removeAction, &QAction::triggered, this, &ToolBar::deleteThis);

	setObjectName("tool");
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
	widget->setParent(m_parent);
	QMessageBox::information(this, "DEBUG", widget->parent()->objectName());
	itemInToolBar.push_back(widget);
	addWidget(widget);
}

void ToolBar::reset()
{
	m_parent->m_urlArea->setParent(m_parent);
	m_parent->m_searchArea->setParent(m_parent);
	m_parent->m_spacer->setParent(m_parent);
	itemInToolBar.clear();
	clear();

	addAction(m_paramAction);
	addAction(m_removeAction);
	addSeparator();
	addSeparator();

}

void ToolBar::deleteThis()
{
	m_parent->deleteToolBar(this);
}

void ToolBar::loadToolBarV0(QTextStream & in)
{

}

void ToolBar::loadToolBarV1(QTextStream & in)
{
	unsigned widgetInToolBar{ 0 };
	unsigned iconSize{ 32 };
	QString currentWidget{ "nothing" };
	QString toolBarPos{ "top" };

	in >> toolBarPos; //< Tool bar position in the window
	in >> iconSize;
	in >> widgetInToolBar; //< Number of widgets in the tool bar from the tooBar.txt file

	if (toolBarPos == "left")
		m_parent->addToolBar(Qt::LeftToolBarArea, this);
	else if (toolBarPos == "right")
		m_parent->addToolBar(Qt::RightToolBarArea, this);
	else if (toolBarPos == "bottom")
		m_parent->addToolBar(Qt::BottomToolBarArea, this);
	else
		m_parent->addToolBar(this);

	setIconSize(QSize(iconSize, iconSize));

	for (size_t i{ 0 }; i < widgetInToolBar; ++i) {
		in >> currentWidget;
		if (currentWidget == "back")
			addNewAction(m_parent->m_backAction);
		else if (currentWidget == "next")
			addNewAction(m_parent->m_nextAction);
		else if (currentWidget == "home")
			addNewAction(m_parent->m_homeAction);
		else if (currentWidget == "refresh")
			addNewAction(m_parent->m_refreshOrStopAction);
		else if (currentWidget == "go")
			addNewAction(m_parent->m_goAction);
		else if (currentWidget == "search")
			addNewAction(m_parent->m_searchAction);
		else if (currentWidget == "history")
			addNewAction(m_parent->m_sowHistory);
		else if (currentWidget == "preferences")
			addNewAction(m_parent->m_preferencesAction);
		else if (currentWidget == "addBookmarks")
			addNewAction(m_parent->m_addBookmarksAction);
		else if (currentWidget == "bookmarksManager")
			addNewAction(m_parent->m_bookmarsManagerAction);
		else if (currentWidget == "newTab")
			addNewAction(m_parent->m_newTabAction);
		else if (currentWidget == "newWin")
			addNewAction(m_parent->m_newWindowAction);
		else if (currentWidget == "urlArea") {
			addNewWidget(m_parent->m_urlArea);
		}
		else if (currentWidget == "searchArea") {
			addNewWidget(m_parent->m_searchArea);
		}
		else if (currentWidget == "separator") {
			addSeparator();
		}
		else if (currentWidget == "spacer") {
			addNewWidget(m_parent->m_spacer);
		}
		else {
			QMessageBox::warning(this, tr("Probleme"), tr("Une erreur est pr�sente � la ligne num�ro ") + QString::number(i + 1) + tr(". "
				"La barre de navigation risque de ne pas se charger comme pr�vu"
				"Nous vous conseillons de contacter le cr�ateur du th�me pour qu'il corrige l'erreur au plus vite."));
		}
	}
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
	m_view->setEditTriggers(QListView::NoEditTriggers);

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
	connect(m_deleteButton, &QPushButton::clicked, this, &ManageToolBar::removeItem);
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
	items << "urlArea" << "searchArea" << "spacer";

	bool ok{ false };
	QString newAction{ QInputDialog::getItem(this, tr("Action � ajouter"), tr("Iteme : "), items, 0, false, &ok) };

	if (ok && !newAction.isEmpty()) {
		if (newAction != "urlArea" &&
			newAction != "searchArea" &&
			newAction != "spacer") {
			QString actionName{ m_parent->m_editableAction[newAction]->objectName() };
			QIcon actionIcon{ m_parent->m_editableAction[newAction]->icon() };
			m_model->appendRow(new QStandardItem(actionIcon, actionName));
		}
		else if (newAction == "urlArea")
			m_model->appendRow(new QStandardItem("urlArea"));
		else if (newAction == "searchArea")
			m_model->appendRow(new QStandardItem("searchArea"));
		else if (newAction == "spacer")
			m_model->appendRow(new QStandardItem("spacer"));
	}
}

void ManageToolBar::removeItem()
{
	QModelIndex index{ m_view->currentIndex() };
	m_model->removeRow(index.row());
}

void ManageToolBar::accept()
{
	m_parentToolBar->reset();

	for (int i{ 0 }; i < m_model->rowCount(); ++i) {
		if (m_model->item(i)->text() != "urlArea" &&
			m_model->item(i)->text() != "searchArea" &&
			m_model->item(i)->text() != "spacer")
			m_parentToolBar->addNewAction(m_parent->m_editableAction[m_model->item(i)->text()]);
		else if (m_model->item(i)->text() == "urlArea") {
			m_parent->m_urlArea = new QLineEdit(m_parent);
			m_parent->m_urlArea->setPlaceholderText(tr("Ici l'url de la page"));
			m_parent->m_urlArea->setObjectName("urlArea");
			m_parentToolBar->addNewWidget(m_parent->m_urlArea);
		}
		else if (m_model->item(i)->text() == "searchArea")
			m_parentToolBar->addNewWidget(m_parent->m_searchArea);
		else if (m_model->item(i)->text() == "spacer")
			m_parentToolBar->addNewWidget(m_parent->m_spacer);
	}

	QMessageBox::information(this, "DEBUG", m_parent->m_urlArea->parent()->objectName());

	close();
}

Qt::DropActions QStandardItemModel::supportedDropActions() const
{
	return Qt::MoveAction;
}


