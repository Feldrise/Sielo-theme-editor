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
	connect(m_saveThmAs, &QAction::triggered, this, &MainWindow::saveThmAs);
	
	connect(m_newToolBar, &QAction::triggered, this, &MainWindow::createNewToolBar);

	m_saveThm->setShortcut(QKeySequence::Save);
	m_saveThmAs->setShortcuts(QKeySequence::SaveAs);

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
	fileMenu->addAction(m_saveThmAs);

	QMenu *themeMenu = menuBar()->addMenu(tr("&Thèmes"));

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
		QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'ouvrir le thème de la barre d'outils"));
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
			connect(m_toolBars[i], &ToolBar::topLevelChanged, this, &MainWindow::unsaveThm);
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

	connect(newToolBar, &ToolBar::topLevelChanged, this, &MainWindow::unsaveThm);

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
	m_thmName = QInputDialog::getText(this, tr("Nom du thème"), tr("Entrez le nom du thème"), QLineEdit::Normal, QString(), &ok);

	if (ok && !m_thmName.isEmpty()) {
		if (!m_thmPath.isEmpty())
			closeThm();

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
	QString filePath{ QFileDialog::getOpenFileName(this, tr("Ouvrir un thème"), QString(), "Sielo Thèmes (*.snthm)") };

	if (!filePath.isEmpty()) {
		if (!m_thmPath.isEmpty())
			closeThm();

		QFileInfo thmInfo{ filePath };
		m_thmPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/SNThemeEditor/" + thmInfo.baseName() + "/";
		m_savedThmPath = filePath;
		QDir *themePath{ new QDir(m_thmPath) };
		themePath->mkpath(m_thmPath);
		ThemeManager::decompressTheme(filePath, themePath->absolutePath());

		loadToolBar(QString(m_thmPath + "toolBar.txt"));
		createActions();
	}
}

void MainWindow::saveThm()
{
	QString version{ "2" };
	QString toolBarNumber{ QString::number(m_toolBars.size()) };

	if (m_savedThmPath.isEmpty()) {
		m_savedThmPath = QFileDialog::getSaveFileName(this, tr("Sauvegarde du thème"), QString(), "Sielo Thème (*.snthm)");

		if (m_savedThmPath.isEmpty()) 
			return;
	}
	QFile toolBarTxt{ m_thmPath + "toolBar.txt" };
	if (!toolBarTxt.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::critical(this, tr("Erreur"), tr("Impossible de sauvegarder le thème"));
		return;
	}

	QTextStream out{&toolBarTxt};
	out << version << ' ' << toolBarNumber << ' ';
	for (int i{ 0 }; i < m_toolBars.size(); ++i) {
		ToolBar *toolBar{ m_toolBars[i] };
		if (toolBarArea(toolBar) == Qt::TopToolBarArea)
			out << "top ";
		else if (toolBarArea(toolBar) == Qt::BottomToolBarArea)
			out << "bottom ";
		else if (toolBarArea(toolBar) == Qt::LeftToolBarArea)
			out << "left ";
		else if (toolBarArea(toolBar) == Qt::RightToolBarArea)
			out << "right ";

		out << QString::number(toolBar->iconSize) << ' ';
		out << QString::number(toolBar->itemInToolBar.size()) << ' ';

		for (int j{ 0 }; j < toolBar->itemInToolBar.size(); ++j)
			out << toolBar->itemInToolBar[j]->objectName() << ' ';
	}

	toolBarTxt.close();

	ThemeManager::compressTheme(m_thmPath, m_savedThmPath);
	thmSaved = true;
}

void MainWindow::saveThmAs()
{
	QString newThmPath = QFileDialog::getSaveFileName(this, tr("Sauvegarde du thème"), QString(), "Sielo Thème (*.snthm)");

	if (newThmPath.isEmpty()) 
		return;
	else {
		m_savedThmPath = newThmPath;
		saveThm();
	}
}

void MainWindow::closeThm()
{
	for (int i{ 0 }; i < m_toolBars.size(); ++i) 
		removeToolBar(m_toolBars[i]);

	m_toolBars.clear();
	m_editableAction.clear();

	QDir themePath{ m_thmPath };
	themePath.removeRecursively();

	m_savePath = QString();
	m_thmPath = QString();
	m_thmName = QString();
	m_savedThmPath = QString();
}

void MainWindow::unsaveThm()
{
	thmSaved = false;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	QMessageBox::information(this, "DEBUG", "Close event");
	if (!thmSaved) {
		QMessageBox::StandardButton save = QMessageBox::question(this, tr("Sauvegarder"), tr("Voulez vous sauvegarder le thème"), QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Close);
		
		if (save == QMessageBox::Save) {
			saveThm();
			closeThm();
			event->accept();
		}
		else if (save == QMessageBox::Close) {
			closeThm();
			event->accept();
		}
		else
			event->ignore();
	}
	else {
		if (!m_thmPath.isEmpty())
			closeThm();
		event->accept();
	}
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