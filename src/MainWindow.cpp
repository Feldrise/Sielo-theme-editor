#include "includes/MainWindow.hpp"

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QStandardPaths>
#include <QDir>

MainWindow::MainWindow(QWidget * parent) : 
	QMainWindow(parent)
{
	createMenus();
	resize(1024, 768);

	connect(m_newThm, &QAction::triggered, this, &MainWindow::newThm);
	connect(m_openThm, &QAction::triggered, this, &MainWindow::openThm);
	connect(m_saveThm, &QAction::triggered, this, &MainWindow::saveThm);

//	QMessageBox::information(this, "DEBUG", QStandardPaths::writableLocation(QStandardPaths::TempLocation));
}

MainWindow::~MainWindow()
{
	// Empty
}

void MainWindow::createMenus()
{
	QMenu *fileMenu = menuBar()->addMenu(tr("&Fichier"));

	fileMenu->addAction(m_newThm);
	fileMenu->addAction(m_openThm);
	fileMenu->addSeparator();
	fileMenu->addAction(m_saveThm);
}

void MainWindow::createActions()
{
		m_backAction->setIcon(QIcon(m_thmPath + "back.png"));
		m_editableAction.insert("back", m_backAction);
		m_nextAction->setIcon(QIcon(m_thmPath + "next.png"));
		m_editableAction.insert("next", m_nextAction);
		m_homeAction->setIcon(QIcon(m_thmPath + "home.png"));
		m_editableAction.insert("home", m_homeAction);
		m_refreshOrStopAction->setIcon(QIcon(m_thmPath + "refresh.png"));
		m_editableAction.insert("refresh", m_refreshOrStopAction);
		m_goAction->setIcon(QIcon(m_thmPath + "go.png"));
		m_editableAction.insert("go", m_goAction);
		m_searchAction->setIcon(QIcon(m_thmPath + "search.png"));
		m_editableAction.insert("search", m_searchAction);
		m_sowHistory->setIcon(QIcon(m_thmPath + "history.png"));
		m_editableAction.insert("history", m_sowHistory);
		m_preferencesAction->setIcon(QIcon(m_thmPath + "preferences.png"));
		m_editableAction.insert("preferences", m_preferencesAction);
		m_addBookmarksAction->setIcon(QIcon(m_thmPath + "addFavoris.png"));
		m_editableAction.insert("addBookmarks", m_addBookmarksAction);
		m_bookmarsManagerAction->setIcon(QIcon(m_thmPath + "favoris.png"));
		m_editableAction.insert("bookmarksManager", m_bookmarsManagerAction);
		m_newTabAction->setIcon(QIcon(m_thmPath + "newTab.png"));
		m_editableAction.insert("newTab", m_newTabAction);
		m_newWindowAction->setIcon(QIcon(m_thmPath + "newWindow.png"));
		m_editableAction.insert("newWindow", m_newWindowAction);
		m_exitAction->setIcon(QIcon(m_thmPath + "exit.png"));
}

ToolBar *MainWindow::addNewToolBar(Qt::ToolBarArea area)
{
	ToolBar *newToolBar{ new ToolBar(this) };
	m_toolBars.push_back(newToolBar);
	addToolBar(area, newToolBar);

	return newToolBar;
}

void MainWindow::newThm()
{
	bool ok{ false };
	m_thmName = QInputDialog::getText(this, tr("Nom du th�me"), tr("Entrez le nom du th�me"), QLineEdit::Normal, QString(), &ok);

	if (ok && !m_thmName.isEmpty()) {
		m_thmPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/SNThemeEditor/" + m_thmName + "/";
		QDir *themePath{ new QDir(m_thmPath) };
		themePath->mkpath(m_thmPath);
		copyDir("SIcons", themePath->absolutePath());
		ToolBar *default = addNewToolBar();
		createActions();
	}
}

void MainWindow::openThm()
{
	ToolBar *t1{ addNewToolBar() };
	ToolBar *t2{ addNewToolBar(Qt::LeftToolBarArea) };
	
	t1->addAction(m_backAction);
	t1->addAction(m_nextAction);

	t2->addAction(m_backAction);

}

void MainWindow::saveThm()
{
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	QMessageBox::information(this, "DEBUG", "Close event");
}

void MainWindow::copyDir(QString src, QString dst)
{
	QDir dir(src);
	if (!dir.exists())
		return;

	foreach(QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
		QString dst_path = dst + QDir::separator() + d;
		dir.mkpath(dst_path);
		copyDir(src + QDir::separator() + d, dst_path);
	}

	foreach(QString f, dir.entryList(QDir::Files)) {
		QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
	}
}