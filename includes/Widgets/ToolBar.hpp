#pragma once

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QToolBar>
#include <QPushButton>
#include <QStandardItemModel>
#include <QListView>
#include <QIcon>
#include <QVector>

class TooBar;

class ManageToolBar : public QDialog
{
public:
	ManageToolBar(QWidget *parent = nullptr, ToolBar *parentToolBar);
	~ManageToolBar();

private:
	QVBoxLayout *m_layout{ new QVBoxLayout(this) };
	QHBoxLayout *m_buttonLayout{ new QHBoxLayout() };

	QListView *m_view{ new QListView(this) };
	QStandardItemModel *m_model{ new QStandardItemModel(m_view) };

	QPushButton *m_newButton{ new QPushButton(QObject::tr("Nouveau"), this) };
	QPushButton *m_deleteButton{ new QPushButton(QObject::tr("Supprimer"), this) };
	QDialogButtonBox *m_boxBtn{ new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this) };

};

class ToolBar : public QToolBar
{
public:
	ToolBar(QWidget *parent = nullptr);
	~ToolBar();

	void reset();

	QVector<QAction*> m_actions{};
private:
	QAction *m_paramAction{ new QAction(QIcon("Images/setting.png"), QObject::tr("Paramètres"), this) };
	QAction *m_removeAction{ new QAction(QIcon("Images/remove.png"), QObject::tr("Supprimer la barre d'outils"), this) };
};