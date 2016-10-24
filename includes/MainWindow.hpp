#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QIcon>
#include <QHash>
#include <QVector>
#include <QCloseEvent>

#include "includes/Widgets/ToolBar.hpp"

class MainWindow : public QMainWindow
{
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void createMenus();
	void createActions();

	void newThm();
	void openThm();
	void saveThm();

	ToolBar* addNewToolBar(Qt::ToolBarArea area = Qt::TopToolBarArea);

	QAction *m_backAction{ new QAction(QObject::tr("&Précédent"), this) };
	QAction *m_nextAction{ new QAction(QObject::tr("&Suivant"), this) };
	QAction *m_homeAction{ new QAction(QObject::tr("&Accueil"), this) };
	QAction *m_refreshOrStopAction{ new QAction(QObject::tr("&Rafraichir"), this) };
	QAction *m_goAction{ new QAction(QObject::tr("&Go"), this) };
	QAction *m_searchAction{ new QAction(QObject::tr("Chercher"), this) };
	QAction *m_sowHistory{ new QAction(QObject::tr("&Historique"), this) };
	QAction *m_preferencesAction{ new QAction(QObject::tr("Pré&férences"), this) };
	QAction *m_addBookmarksAction{ new QAction(QObject::tr("&Nouveau favori"), this) };
	QAction *m_bookmarsManagerAction{ new QAction(QObject::tr("&Gérer les favoris")) };
	QAction *m_newTabAction{ new QAction(QObject::tr("Nouvel onglet"), this) };
	QAction *m_newWindowAction{ new QAction(QObject::tr("Nouvelle fenêtre"), this) };
	QAction *m_exitAction{ new QAction(QObject::tr("Fermer le navigateur"), this) };

	QHash<QString, QAction*> m_editableAction{};
protected:
	void closeEvent(QCloseEvent *event);

private:
	void copyDir(QString src, QString dst);

	QString m_savePath{};
	QString m_thmPath{};
	QString m_thmName{};

	// Actions for menus
	QAction *m_newThm{ new QAction(QObject::tr("Nouveau thème"), this) };
	QAction *m_openThm{ new QAction(QObject::tr("Ouvrir un thème"), this) };
	QAction *m_saveThm{ new QAction(QObject::tr("Sauvegarder le thème"), this) };

	QVector<ToolBar*> m_toolBars{};
};