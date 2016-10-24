#include "includes/MainWindow.hpp"
#include "includes/ThemeManager.hpp"

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>

const unsigned int THEME_V0 = 1;
const unsigned int THEME_V1 = 2;

MainWindow::MainWindow(QWidget * parent) : 
	QMainWindow(parent)
{
	createMenus();
	resize(1024, 768);

	connect(m_newThm, &QAction::triggered, this, &MainWindow::newThm);
	connect(m_openThm, &QAction::triggered, this, &MainWindow::openThm);
	connect(m_saveThm, &QAction::triggered, this, &MainWindow::saveThm);
	
	connect(m_newToolBar, &QAction::triggered, this, &MainWindow::createNewToolBar);

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

	QMenu *themeMenu = menuBar()->addMenu(tr("&Th�mes"));

	themeMenu->addAction(m_newToolBar);
}

void MainWindow::createActions()
{
		m_backAction->setIcon(QIcon(m_thmPath + "back.png"));
		m_backAction->setObjectName("back");
		m_editableAction.insert("back", m_backAction);
		m_nextAction->setIcon(QIcon(m_thmPath + "next.png"));
		m_nextAction->setObjectName("next");
		m_editableAction.insert("next", m_nextAction);
		m_homeAction->setIcon(QIcon(m_thmPath + "home.png"));
		m_homeAction->setObjectName("home");
		m_editableAction.insert("home", m_homeAction);
		m_refreshOrStopAction->setIcon(QIcon(m_thmPath + "refresh.png"));
		m_refreshOrStopAction->setObjectName("refresh");
		m_editableAction.insert("refresh", m_refreshOrStopAction);
		m_goAction->setIcon(QIcon(m_thmPath + "go.png"));
		m_goAction->setObjectName("go");
		m_editableAction.insert("go", m_goAction);
		m_searchAction->setIcon(QIcon(m_thmPath + "search.png"));
		m_searchAction->setObjectName("search");
		m_editableAction.insert("search", m_searchAction);
		m_sowHistory->setIcon(QIcon(m_thmPath + "history.png"));
		m_sowHistory->setObjectName("history");
		m_editableAction.insert("history", m_sowHistory);
		m_preferencesAction->setIcon(QIcon(m_thmPath + "preferences.png"));
		m_preferencesAction->setObjectName("preferences");
		m_editableAction.insert("preferences", m_preferencesAction);
		m_addBookmarksAction->setIcon(QIcon(m_thmPath + "addFavoris.png"));
		m_addBookmarksAction->setObjectName("addBookmarks");
		m_editableAction.insert("addBookmarks", m_addBookmarksAction);
		m_bookmarsManagerAction->setIcon(QIcon(m_thmPath + "favoris.png"));
		m_bookmarsManagerAction->setObjectName("bookmarksManager");
		m_editableAction.insert("bookmarksManager", m_bookmarsManagerAction);
		m_newTabAction->setIcon(QIcon(m_thmPath + "newTab.png"));
		m_newTabAction->setObjectName("newTab");
		m_editableAction.insert("newTab", m_newTabAction);
		m_newWindowAction->setIcon(QIcon(m_thmPath + "newWindow.png"));
		m_newWindowAction->setObjectName("newWindow");
		m_editableAction.insert("newWindow", m_newWindowAction);
		m_exitAction->setIcon(QIcon(m_thmPath + "exit.png"));


}

void MainWindow::loadToolBar(QString & filePath)
{
	QFile file{ filePath };

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'ouvrir le th�me de la barre d'outils"));
	}

	QTextStream in{ &file };

	unsigned version{ 0 };
	in >> version;

	unsigned nbreToolBar{ 0 };
	switch (version)
	{
	case THEME_V0:
		in >> nbreToolBar;

		for (size_t i{ 0 }; i < nbreToolBar; ++i) {
			ToolBar *newToolBar{ addNewToolBar() };
			newToolBar->loadToolBarV0(in);
		}
		break;
	case THEME_V1:
		in >> nbreToolBar;

		for (size_t i{ 0 }; i < nbreToolBar; ++i) {
			m_toolBars.push_back(new ToolBar(this));
			m_toolBars[i]->loadToolBarV1(in);
		}
		break;
	default:
		QMessageBox::critical(this, tr("Erreur"), tr("La version ") + QString::number(version) + tr(" est inconnue"));
		break;
	}
}

ToolBar *MainWindow::addNewToolBar(Qt::ToolBarArea area)
{
	ToolBar *newToolBar{ new ToolBar(this) };
	m_toolBars.push_back(newToolBar);
	addToolBar(area, newToolBar);

	return newToolBar;
}

void MainWindow::createNewToolBar()
{
	QToolBar *newToolBar{ addNewToolBar() };
}

void MainWindow::deleteToolBar(ToolBar *toolBar)
{
	removeToolBar(toolBar);
	m_toolBars.remove(m_toolBars.indexOf(toolBar));
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
	QString filePath{ QFileDialog::getOpenFileName(this, tr("Ouvrir un th�me"), QString(), "Sielo Th�mes (*.snthm)") };

	if (!filePath.isEmpty()) {
		QFileInfo thmInfo{ filePath };
		m_thmPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/SNThemeEditor/" + thmInfo.baseName() + "/";
		QDir *themePath{ new QDir(m_thmPath) };
		themePath->mkpath(m_thmPath);
		ThemeManager::decompressTheme(filePath, themePath->absolutePath());

		loadToolBar(QString(m_thmPath + "toolBar.txt"));
		createActions();
	}
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